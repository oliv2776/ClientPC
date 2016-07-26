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
#define _CRT_SECURE_NO_WARNINGS
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
#include <time.h>

/*Defines*/
#define PORT 23
#define BOARDNAME1 'A'
#define BOARDNAME2 '0'
#define ADC1 '1'
#define ADC2 '2'
#define ADC3 '3'
#define SIZEDATA 1024
#define SIZETRAME SIZEDATA+30

int formatbuffer(char buffer[],int nbPacket, int numberTotalOfPackets);
int convertCharToInt(char charToConvert[], int index);

int main()
{
	SOCKET sock;
	SOCKADDR_IN sin;
	char bufferServer[32] = "";// buffer for the commandes sent by the server
	/*Signals sent: buffer[0]= board ID; buffer[1]=ADC number; 
	buffer[2] = packet number; buffer[3] = total number of packets; 
	buffer[4] = day; buffer[5] = month; buffer[6]= year; buffer[7]= hour; 
	buffer[8]= min; buffer[9]= sec; buffer[10] = milliseconds*/
	char bufferADC1[32], bufferADC2[32], bufferADC3[32];
	char tempBuffer[SIZEDATA];
	for (int i = 0; i < SIZEDATA; i++) {
		tempBuffer[i] = '0';
	}
	int timeConversion;
	SYSTEMTIME t;
	GetSystemTime(&t);
	int index;

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
			printf("time: %dh %dmin %dsec\n",t.wHour,t.wMinute,t.wSecond);
			/*If we receive a message we display it on the screen*/
			while (bufferServer[0] != 'z') {
				if (recv(sock, bufferServer, sizeof(bufferServer), 0) != 0) {
					printf("receiving data!\n");

					switch (bufferServer[0]) {
					case 'a':
						printf("start adc1\n");
						timeConversion = convertCharToInt(bufferServer, 1);
						printf("The conversion will last %dmin\n", timeConversion);
						for (int i = 0; i < timeConversion; i++) {
							index = formatbuffer(bufferADC1,i, timeConversion);
							for (int j = 0; j < SIZEDATA; j++) {
								bufferADC1[j + index] = tempBuffer[j];
							}
							for (int k = 0; k < SIZETRAME; k++) {
								printf("%c\t", bufferADC1);
								printf("\n");
							}

						}
						
						
						break;
					case 'b':
						printf("start adc2\n");
						timeConversion = bufferServer[1];
						printf("The conversion will last %dmin\n", timeConversion);
						bufferADC2[3] = timeConversion;
						for (int i = 0; i < timeConversion; i++) {
							bufferADC2[4] = i;
							send(sock, bufferADC2, 32, 0);
						}
						break;
					case 'c':
						printf("start adc3\n");
						timeConversion = bufferServer[1];
						printf("The conversion will last %dmin\n", timeConversion);
						bufferADC3[3] = timeConversion;
						for (int i = 0; i < timeConversion; i++) {
							bufferADC3[4] = i;
							send(sock, bufferADC3, 32, 0);
						}
						break;

					default:
						printf("%s\n", bufferServer);
						break;
					}
				}
			}
			
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

 int formatbuffer(char buffer[], int nbPacket,int numberTotalOfPackets) {
	SYSTEMTIME t;
	GetSystemTime(&t);
	/*Configuring the buffers*/
	buffer[0] = BOARDNAME1;
	buffer[1] = BOARDNAME2;
	buffer[2] = ADC1;
	int index = 3;
	char buf[6];
	sprintf(buf, "%d", nbPacket);
	for (int i = 0; i < 6; i++) {
		printf("%c", buf[i]);
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", numberTotalOfPackets);
	for (int i = 0; i < 6; i++) {
		printf("%c", buf[i]);
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wDay);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wMonth);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wYear);
	for (int i = 0; i < 4; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wHour);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wMinute);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wSecond);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	sprintf(buf, "%d", t.wMilliseconds);
	for (int i = 0; i < 2; i++) {
		buffer[index] = buf[i];
		index++;
	}
	return index;
}

 int convertCharToInt(char charToConvert[], int index) {
	 int result;
	 result = charToConvert[index] * 127 + charToConvert[index + 1];
	 return result;
 }