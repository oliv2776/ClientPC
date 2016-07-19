// ClientPC.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
/*To avoid _WINSOCK_DEPRECATED_NO_WARNINGS error while compiling*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

typedef int socklen_t;
using namespace std;

#define PORT 23

int main()
{
	WSADATA WSAData;
	int error = WSAStartup(MAKEWORD(2,2), &WSAData);
	SOCKET sock;
	SOCKADDR_IN sin;

	if (!error) {
		/*Create the socket*/
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/*Configuring connection*/
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(PORT);

		/*If the client connect*/
		if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
			printf("Connecting to %s on the port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
		else {
			printf("impossible to connect\n");
			while (1);
		}

		/*Closing the socket*/
		closesocket(sock);

		WSACleanup();
	}
	return EXIT_SUCCESS;
}