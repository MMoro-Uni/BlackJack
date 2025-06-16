/*!
* \file blackjackclient.c
* \brief contains functions for handling the blackjack server.
* The code in this file is not meant for usage in other projects. Don't include without changes.
* Global variables are used to avoid deallocation and reallocation of memory when not needed.
* If changing the code to support a server different from localhost, this design would not work and changes would be needed.
* 
* \fn void close_connection()
* \brief small util function to improve readability
* 
* \fn int client_connection_init()
* \brief connects the client to the server.
* client connects always to port 27015 of localhost through an IPv4 IP.
* return value is 1 if errors happened, 0 if connection was successful.
* 
* \fn int server_login(char username[], char password[], MODE mode)
* \brief starts the login process or the register process based on mode.
* The client will first send an "Username:<value>" query, which tells the server to save the given username for use.
* It will then send a "Password:<value>" query, which tells the server to save the given password for use.
* In the end, either a "Login" or a "Register" query is sent, which tells the server what operation to execute.
* The server can return "logok" or "logfail" for login and "regok" or "regfail" for register. Those values are used to determine if operation was successful.
* Return value is 1 if errors happened, 0 if connection was successful.
* 
* \fn int money_value_update(int modify_value)
* \brief changes the amount of money the player has by modify_value
* A "Modify:<value>" query is sent to the server, which then changes the amount of money the current user has by <value>.
* For the modify query the server uses the username and password that were saved during the login process.
* To avoid unintended behaviour, always call this function when sure that an user completed the login process.
* Return value is 1 if errors happened, 0 if connection was successful.
* 
* \fn int money_value_get()
* \brief gets the amount of money the player has.
* The client sends a "GetVal" query to the server, which then returns a string with the amount of money the player has.
* The server uses the username and password that were saved during the login process.
* To avoid unintended behaviour, always call this function when sure that an user completed the login process.
* Return value is the amount of money the player has
* NOTE: in the current implementation, the function returns 1 in case of failure. This can be noticed since the value will not change with wins or losses in the game.
*/


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string.h>
#include "blackjackclient.h"

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 128

//global variable declaration
static WSADATA wsaData;
static int iResult;
static struct addrinfo* result = NULL, * ptr = NULL, hints;
static SOCKET ConnectSocket;
static char qbuffer[DEFAULT_BUFLEN] = "";
static char recvbuf[DEFAULT_BUFLEN] = "";
static int init_result;

void close_connection();
int client_connection_init();

void close_connection()
{
    closesocket(ConnectSocket);
    WSACleanup();
}

int client_connection_init()
{
    //start library
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    //setup socket
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    
    //create socket
    ConnectSocket = INVALID_SOCKET;
    ptr = result;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    //connect to server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    return 0;
}

int server_login(char username[], char password[], MODE mode)
{
    init_result = client_connection_init();

    if (init_result == 1)
    {
        return 1;
    }

    //send username to save
    sprintf_s(qbuffer, DEFAULT_BUFLEN, "Username:%s", username);

    iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }

    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult < 0)
    {
        printf("recv failed: %d\n", WSAGetLastError());
        return 1;
    }
    else if (iResult == 0)
    {
        printf("server connection closed without warning\n");
        return 1;
    }

    //send password to save
    sprintf_s(qbuffer, DEFAULT_BUFLEN, "Password:%s", password);

    iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }

    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult < 0)
    {
        printf("recv failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }
    else if (iResult == 0)
    {
        printf("server connection closed without warning\n");
        close_connection();
        return 1;
    }

    //login or register
    if (mode == LOGIN_MODE)
    {

        strcpy(qbuffer, "Login");

        iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed: %d\n", WSAGetLastError());
            close_connection();
            return 1;
        }

        iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

        if (strcmp(recvbuf, "logok") == 0)
        {
            printf("login successful\n");

            iResult = shutdown(ConnectSocket, SD_SEND);

            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed: %d\n", WSAGetLastError());
                close_connection();
                return 1;
            }

            closesocket(ConnectSocket);
            WSACleanup();
            return LOGIN_SUCCESS;
        }
        else if (strcmp(recvbuf, "logfail") == 0)
        {
            printf("login failed\n");

            iResult = shutdown(ConnectSocket, SD_SEND);

            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed: %d\n", WSAGetLastError());
                close_connection();
                return 1;
            }

            close_connection();
            return LOGIN_FAIL;
        }
    }
    else if (REGISTER_MODE)
    {

        strcpy(qbuffer, "Register");

        iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed: %d\n", WSAGetLastError());
            close_connection();
            return 1;
        }

        iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

        if (strcmp(recvbuf, "regok") == 0)
        {
            printf("register successful\n");

            iResult = shutdown(ConnectSocket, SD_SEND);

            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed: %d\n", WSAGetLastError());
                close_connection();
                return 1;
            }

            return REGISTER_SUCCESS;
        }

        close_connection();
        return REGISTER_FAIL;
    }
}

int money_value_update(int modify_value)
{

    init_result = client_connection_init();

    if (init_result == 1)
    {
        return 1;
    }
    
    //send query to server
    sprintf_s(qbuffer, DEFAULT_BUFLEN, "Modify:%d", modify_value);

    iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());

        close_connection();
        return 1;
    }

    //check operation success
    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult < 0)
    {
        printf("recv failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }
    else if (iResult == 0)
    {
        printf("server connection closed without warning\n");
        close_connection();
        return 1;
    }

    close_connection();
    return 0;
}

int money_value_get()
{
    init_result = client_connection_init();

    if (init_result == 1)
    {
        return 1;
    }

    //send query to server
    strcpy(qbuffer, "GetVal");

    iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }

    //return value
    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

    if (strcmp(recvbuf, "") != 0)
    {
        int return_value = (int)strtol(recvbuf, recvbuf[strlen(recvbuf)], 10);
        close_connection();
        return return_value;
    }

    printf("failure in receiving money amount\n");
    close_connection();
    return 0;
}



