/*!
*\file ClientPC.cpp
*\brief Client PC to connect to the server
*\author Olivier Deschênes
*\version 0.1
*/

#if defined (WIN32)
typedef int socklen_t;
#include "stdafx.h"
/*To avoid _WINSOCK_DEPRECATED_NO_WARNINGS error while compiling*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string>




#define PORT 23

int main()
{
	SOCKET sock;
	SOCKADDR_IN sin;
	char buffer[32] = "";
	char buf;
#if defined (WIN32)
	WSADATA WSAData;
	int error = WSAStartup(MAKEWORD(2, 2), &WSAData);
#else
	int error = 0;
#endif

	if (!error) {
		/*Create the socket*/
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/*Configuring connection*/
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(PORT);

		/*If the client connect*/
		if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR) 
		{
			printf("Connecting to %s on the port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
			/*If we receive a message we display it on the screen*/
			do {
				printf("receiving data!\n");
			
				switch (buffer[0]) {
				case 1:
					printf("start adc1");
					break;
				default:
					printf("%s\n", buffer);
					break;
					}

				
			} while (recv(sock, buffer, 32, 0) !=0);
		}
		else {
			printf("impossible to connect\n");
		}

		/*Closing the socket*/
		closesocket(sock);

#if defined (WIN32)
		WSACleanup();
#endif
	}
	/*Waiting for the user to type something to close*/
	getchar();

	return EXIT_SUCCESS;
}