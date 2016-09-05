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
#define BOARDNAME 1
#define ADC1 '1'
#define ADC2 '2'
#define ADC3 '3'
#define SIZEDATA 1024
#define SIZEFRAME 2*SIZEDATA+21


union frame_u {
	struct frame_s {
		uint8_t board;
		uint8_t adc_number;
		uint32_t packet_number;
		uint32_t total_of_packet;
		uint8_t day;
		uint8_t month;
		uint8_t year;
		uint8_t hour;
		uint8_t minutes;
		uint8_t seconds;
		uint8_t miliseconds;
		uint32_t data_lenght;
		uint16_t data[SIZEDATA];
	}frame_as_field;
	char frame_as_byte[SIZEFRAME];
};


void formatbuffer(union frame_u frame, uint8_t adcnumber, uint32_t nbPacket, uint32_t numberTotalOfPackets);
/**
* @brief convert a table of chars into int
* @param char charToConvert[] the table to convert
* @param int index: the index in the table of char where there is the lengh of the number to convert
* @retval uint_32 return the error code: 0= no errors; 1= wrong switch numbe
*/


int main()
{
	SOCKET sock;
	SOCKADDR_IN sin;
	SYSTEMTIME t;
	GetSystemTime(&t);
	char bufferServer[SIZEFRAME];
	char bufferADC1[32], bufferADC2[32], bufferADC3[32];
	char tempBuffer[SIZEFRAME];
	
	uint32_t timeConversion;
	int index;

	union frame_u frame_buf;

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

					for (int i = 0; i < SIZEFRAME; i++) {
						frame_buf.frame_as_byte[i] = bufferServer[i];
						printf("%d \t", frame_buf.frame_as_byte[i]);
					}
					printf("\n number %d, adc number: %d, time conversion: %d\n",frame_buf.frame_as_field.board, frame_buf.frame_as_field.adc_number, frame_buf.frame_as_field.data_lenght);
					printf("%d, %d, %d\n", frame_buf.frame_as_field.day, frame_buf.frame_as_field.month, frame_buf.frame_as_field.year);
				}
			}
			
		}
		else {
			printf("impossible to connect\n");
		}

		/*Closing the socket*/
		//closesocket(sock);

#if defined (WIN32)
		WSACleanup();
#endif
	}
	/*Waiting for the user to type something to close*/
	getchar();

	return EXIT_SUCCESS;
}

 void formatbuffer(union frame_u frame, uint8_t adcnumber, uint32_t nbPacket, uint32_t numberTotalOfPackets) {
	SYSTEMTIME t;
	GetSystemTime(&t);

	frame.frame_as_field.board = BOARDNAME;
	frame.frame_as_field.adc_number = adcnumber;
	frame.frame_as_field.packet_number = nbPacket;
	frame.frame_as_field.total_of_packet = numberTotalOfPackets;
	frame.frame_as_field.day = t.wDay;
	frame.frame_as_field.month = t.wMonth;
	frame.frame_as_field.year = t.wYear;
	frame.frame_as_field.hour = t.wHour;
	frame.frame_as_field.minutes = t.wMinute;
	frame.frame_as_field.seconds = t.wSecond;
	frame.frame_as_field.miliseconds = t.wMilliseconds;
	

	
}

