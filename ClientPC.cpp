// ClientPC.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int main()
{
	int iPadress = 0;
	do {
		cout << "Enter the IP adress of the server\n";
		cin >> iPadress;
	} while (iPadress <= 10000);
	return 0;
}