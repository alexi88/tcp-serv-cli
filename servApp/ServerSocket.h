#pragma once
#include <stdio.h>
#include <winsock2.h>
#include<psapi.h>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<memory>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Psapi.lib")

using namespace std;
class cServerSocket
{
	SOCKET m_hServerSocket;
	SOCKET m_hClientSocket;
	std::map<int, SOCKET> m_ClientsList;
	HANDLE hClientListMutex;
public:

#pragma region C-tors/D-tors

	cServerSocket(){};
	~cServerSocket(){};

#pragma endregion

#pragma region Methods

	DWORD ListenForClients(DWORD u32_ServerIP, USHORT u16_ServerPort);
	DWORD AcceptNewClient(int id);

	DWORD SendPackage(int clientId, char* buff, int buffSize);
	DWORD ReceivePackage(char* package, int packageSize);
	vector<int> GetClientsIds();
	SOCKET GetClientSocket(int clientId);
	void   RemoveClosedSocket(int clienId);
private:

	DWORD Initialize();
	DWORD CreateSocket();

#pragma endregion
};