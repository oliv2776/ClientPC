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
#define ADC1 1
#define ADC2 2
#define ADC3 3
#define ADC1_2 4
#define ADC1_3 5
#define ADC2_3 6
#define ADC1_2_3 7
#define SIZEDATA 1024
#define SIZEFRAME 2*SIZEDATA+24


union frame_u {
	struct frame_s {
		uint8_t board;
		uint8_t adc_number;
		uint32_t packet_number;
		uint32_t total_of_packet;
		uint8_t day;
		uint8_t month;
		uint16_t year;
		uint8_t hour;
		uint8_t minutes;
		uint8_t seconds;
		uint8_t miliseconds;
		uint32_t data_lenght;
		uint16_t data[SIZEDATA];
	}frame_as_field;
	char frame_as_byte[SIZEFRAME];
};

int send_command(int menu_choice, int socket_error, SOCKET csocket);
void send_data(SOCKET csocket, uint8_t adc_number, uint32_t packet_number, uint32_t total_of_packet, uint16_t data[SIZEDATA]);
frame_u formatbuffer(uint8_t boardNumber, uint8_t adcnumber, uint32_t nbPacket, uint32_t numberTotalOfPackets);
int main()
{
	SOCKET sock;
	SOCKADDR_IN sin;
	SYSTEMTIME t;
	GetSystemTime(&t);
	char bufferServer[SIZEFRAME];

	uint32_t number_total_of_packets;
	uint32_t current_number_of_packet;
	uint16_t dat[SIZEDATA];
	int adc_number;

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
			do {
				if (recv(sock, frame_buf.frame_as_byte, sizeof(frame_buf.frame_as_byte), 0) != 0) {
					printf("receiving data!\n");
					number_total_of_packets = frame_buf.frame_as_field.total_of_packet;

					/*
					//To verify if the frame is correct.
					printf("\n\n%lu %lu\n", number_total_of_packets,frame_buf.frame_as_field.total_of_packet);
					printf("\n number %d, adc number: %d, time conversion: %lu\n",frame_buf.frame_as_field.board, frame_buf.frame_as_field.adc_number, frame_buf.frame_as_field.data_lenght);
					printf("%d, %d, %d\n", frame_buf.frame_as_field.day, frame_buf.frame_as_field.month, frame_buf.frame_as_field.year);
					printf("\n%lu\t%lu\t%lu\n", frame_buf.frame_as_field.packet_number, frame_buf.frame_as_field.total_of_packet, frame_buf.frame_as_field.data_lenght);
					*/

					adc_number = frame_buf.frame_as_field.adc_number;
					switch (adc_number) {
					case ADC1:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 1 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC1, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					case ADC2:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 2 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC2, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						
						break;

					case ADC3:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 3 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC3, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					case ADC1_2:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 1 and 2 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC1_2, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					case ADC1_3:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 1 and 3 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC1_3, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					case ADC2_3:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 2 and 3 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC2_3, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					case ADC1_2_3:
						//Start ADC for number_total_of_packet
						printf("\nStart ADC number 1,2 and 3 for %lu packets\n", number_total_of_packets);
						for (uint32_t i = 0; i < number_total_of_packets; i++) {
							send_data(sock, ADC1_2_3, i, number_total_of_packets, dat);
						}
						printf("\ndone\n");
						break;

					default:
						printf("\nerror, invalid adc number\n");
						break;
					}


				}

			} while (adc_number != 9);
			/*Closing the socket*/
			closesocket(sock);

#if defined (WIN32)
			WSACleanup();
#endif
		}
		/*Waiting for the user to type something to close*/
		getchar();

		}
		else {
			printf("impossible to connect\n");
		}

		
		
	return EXIT_SUCCESS;
}

frame_u formatbuffer(uint8_t boardNumber, uint8_t adcnumber, uint32_t nbPacket, uint32_t numberTotalOfPackets) {
	SYSTEMTIME t;
	GetSystemTime(&t);

	frame_u frame;

	frame.frame_as_field.board = boardNumber;
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
	frame.frame_as_field.data_lenght = SIZEDATA;

	return frame;
}



 int send_command(int menu_choice, int socket_error, SOCKET csocket) {
	 uint32_t total_of_packet;
	 frame_u frame;

	 printf("How many packets do you want?\n");
	 cin >> total_of_packet;

	 //format the frame before being sent 
	 frame = formatbuffer(1, menu_choice, 0, total_of_packet);

	 socket_error = send(csocket, frame.frame_as_byte, SIZEFRAME, 0);
	 if (socket_error = SOCKET_ERROR)
	 {
		 printf("error while sending informations, error %d\n", socket_error);
	 }
	 if (menu_choice == 9) {
		 return 9;
	 }
	 else {
		 return 0;
	 }

 }

 void send_data(SOCKET csocket, uint8_t adc_number,uint32_t packet_number, uint32_t total_of_packet, uint16_t data[SIZEDATA]) {
	 frame_u frame;
	 int socket_error;
	 char tmp[200];
	 frame = formatbuffer(BOARDNAME, adc_number, packet_number, total_of_packet);
	 for (int i = 0; i < SIZEDATA; i++) {
		 frame.frame_as_field.data[i] = data[i];
	 }
	 /*
	 //To verifie if the correct values are written 
	 sprintf(tmp, "%03u-%03u-%06u-%06u-%06u.dat", frame.frame_as_field.board, frame.frame_as_field.adc_number, frame.frame_as_field.day, frame.frame_as_field.month, frame.frame_as_field.year);
	 printf("\n%s\n", tmp);
	 */
	 socket_error = send(csocket, frame.frame_as_byte, SIZEFRAME, 0);
	 if (socket_error == SOCKET_ERROR)
	 {
		 printf("error while sending informations, error %d\n", socket_error);
	 }
	 else {
		 if (socket_error != SIZEFRAME) {
			 printf("wrong size sent %d instead of %d \n", socket_error, SIZEFRAME);
		 }
		 else {
			 printf("frame sent\n");
		 }
	 }

	 //To inform the user when the transfert is half way
	 if (frame.frame_as_field.total_of_packet % 2 == 0) {
		 if (frame.frame_as_field.packet_number / 2 == frame.frame_as_field.total_of_packet) {
			 printf("\nprogress: 50%\n");
		 }
	 }
	 else {
		 if ((frame.frame_as_field.packet_number + 1) / 2 == (frame.frame_as_field.total_of_packet + 1) / 2) {
			 printf("\nprogress:50%\n");
		 }
	 }

 }