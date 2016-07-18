// ClientPC.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	int iPadress = 0;
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	SOCKET sock;
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4148);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));
	/*do {
		cout << "Enter the IP adress of the server\n";
		cin >> iPadress;
	} while (iPadress <= 10000);*/
	return 0;
}