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

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

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

    sprintf_s(qbuffer, DEFAULT_BUFLEN, "Modify:%d", modify_value);

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

    strcpy(qbuffer, "GetVal");

    iResult = send(ConnectSocket, qbuffer, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        close_connection();
        return 1;
    }

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



