#pragma once
#include <stdio.h>
#include <winsock2.h>
#include<psapi.h>
#include<iostream>
#include<string>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Psapi.lib")

using namespace std;
class cClientSocket
{
	SOCKET m_hSocket;
public:

	#pragma region C-tors/D-tors

	cClientSocket(){};
	~cClientSocket(){};

	#pragma endregion

	#pragma region Methods

	DWORD Initialize();
	DWORD CreateSocket();

	DWORD ConnectTo(DWORD u32_ServerIP, USHORT u16_ServerPort);
	DWORD SendPackage(char* package, int packageSize);
	DWORD ReceivePackage(char* package, int packageSize);

	#pragma endregion
};