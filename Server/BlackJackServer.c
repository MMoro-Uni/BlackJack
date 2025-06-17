/*!
* \file BlackJackServer.c
* \brief Contains the code for handling sockets and accepting client connections
*		 Current code will only work for localhost and for this project.
*/

/*!
* \fn int main(int argc char* argv[])
* \brief server loop is inside main since no other function was needed.
*		 when starting the program, the loop starts automatically.
* The function handles the setup of the server, the creation of a listen socket and the connection to a client once a request comes.
* Until the program is manually closed, the server will keep running.
* Once a connection is closed with a client, the server will go back to listening.
* Currently, the server is only capable of handling one request at a time, which doesn't create issues since the project runs on localhost.
* Sockets will have an IPv4 address and will be on port 27015 of localhost.
* 
* signals sent by the server:
* "user": indicates that the username sent by client was received correctly
* "pswd": indicates that the password sent by client was received correctly
* "logok": indicates that the login process finished and a user was found
* "logfail": indicates that the login process finished but an uses wasn't found
* "regok": indicates that the registration process finished successfully
* "updated": indicates the amount of money has been updated correctly
* "<value>: returns the amount of money the player has
*/

/* signals received by the server:
* "Username:<username>": tells the server to save <username> as username for login
* "Password:<password>": tells the server to save <password> as password for login
* "Login": tells the server to start the login process
* "Register": tells the server to start the register process
* "Modify:<value>": tells the server to change the amount of money the player has by <value>
* "GetVal": tells the server to give the client the amount of money the player has
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "dbfunctions.h"

#pragma comment(lib, "Ws2_32.lib") //only needed for Visual C++. Code should be compatible with it, so it will be kept in the code even if it is currently useless.

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define REDUCED_BUFLEN 65
#define USER_PSWD_STR_SIZE 9
#define LOG_STR_SIZE 5
#define REG_STR_SIZE 8
#define MOD_STR_SIZE 7
#define VAL_STR_SIZE 6

int main(int argc, char* argv[])
{
	/*
	* variable setup
	*/

	sqlite3* ConnectedDB;
	int sql_returned = sqlite3_open("BlackJackDB.db", &ConnectedDB);

	if (sql_returned != SQLITE_OK)
	{
		printf("dberror\n");
		return 1;
	}
	
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	char recvbuf[DEFAULT_BUFLEN] = "";
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	char UsernameBuf[REDUCED_BUFLEN] = "";
	char PasswordBuf[REDUCED_BUFLEN] = "";
	char NumberBuf[REDUCED_BUFLEN] = "";
	/*
	* Server loop start
	*/
	while (1)
	{

		/*
		* socket setup and connection with client
		*/
		
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (ListenSocket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(result);

		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			printf("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}


		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		closesocket(ListenSocket);

		

		/*
		* handling of data received from connected client
		*/

		do
		{
			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				printf("Bytes received: %d\n%s\n", iResult, recvbuf);
				
				if (strncmp("Username:", recvbuf, USER_PSWD_STR_SIZE) == 0) //username save
				{
					for (int i = 0; i < strlen(recvbuf) - USER_PSWD_STR_SIZE; i++)
					{
						UsernameBuf[i] = recvbuf[i + USER_PSWD_STR_SIZE];
					}
					iSendResult = send(ClientSocket, "user", iResult, 0);
					if (iSendResult == SOCKET_ERROR)
					{
						printf("send failed: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
				}
				else if (strncmp("Password:", recvbuf, USER_PSWD_STR_SIZE) == 0) //password save
				{
					for (int i = 0; i < strlen(recvbuf) - USER_PSWD_STR_SIZE; i++)
					{
						PasswordBuf[i] = recvbuf[i + USER_PSWD_STR_SIZE];
					}
					iSendResult = send(ClientSocket, "pswd", iResult, 0);
					if (iSendResult == SOCKET_ERROR)
					{
						printf("send failed: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
				}
				else if (strncmp("Login", recvbuf, LOG_STR_SIZE) == 0) //login with saved creds
				{

					int login_result = GameLogin(UsernameBuf, PasswordBuf, ConnectedDB);
					if (login_result == DB_CONN_FAIL || login_result == DB_ERROR)
					{
						printf("db error.\n");
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
					else if (login_result == USER_FOUND)
					{
						iSendResult = send(ClientSocket, "logok", iResult, 0);
						if (iSendResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
					}
					else if (login_result == NO_USER_FOUND)
					{
						iSendResult = send(ClientSocket, "logfail", iResult, 0);
						if (iSendResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
					}

				}
				else if (strncmp("Register", recvbuf, REG_STR_SIZE) == 0) //register with saved creds
				{
					int register_result = GameRegister(UsernameBuf, PasswordBuf, ConnectedDB);
					if (register_result == DB_CONN_FAIL || register_result == DB_ERROR)
					{
						printf("db error.\n");
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
					else if (register_result == REG_DONE)
					{
						iSendResult = send(ClientSocket, "regok", iResult, 0);
						strcpy(UsernameBuf, "");
						strcpy(PasswordBuf, "");
						if (iSendResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
					}
				}
				else if (strncmp("Modify:", recvbuf, MOD_STR_SIZE) == 0) //change money value of logged in user
				{
					for (int i = 0; i < strlen(recvbuf) - MOD_STR_SIZE; i++)
					{
						NumberBuf[i] = recvbuf[i + MOD_STR_SIZE];
					}
					int val_to_pass = (int)strtol(NumberBuf, NULL, 10);
					int alter_result = AlterMoney(UsernameBuf, PasswordBuf, val_to_pass, ConnectedDB);

					if (alter_result == UPDATE_DONE)
					{
						iSendResult = send(ClientSocket, "updated", iResult, 0);
						if (iSendResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
					}
					else
					{
						printf("db error\n");
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}

					
				}
				else if (strncmp("GetVal", recvbuf, VAL_STR_SIZE) == 0) //get money value of logged in user
				{
					int get_money_result = GetMoney(UsernameBuf, PasswordBuf, NumberBuf, ConnectedDB);

					if (get_money_result == OBTAIN_DONE)
					{
						iSendResult = send(ClientSocket, NumberBuf, strlen(NumberBuf), 0);

						if (iSendResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(ClientSocket);
							WSACleanup();
							return 1;
						}
					}
					else
					{
						printf("db error\n");
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
				}

			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n");
			}
			else
			{
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		} while (iResult > 0);

		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	/*
	* end of server loop
	*/

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}