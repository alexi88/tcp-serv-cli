#include "ClientSocket.h"

#pragma region C-tors/D-tors


#pragma endregion

#pragma region Methods

DWORD cClientSocket::Initialize()
{
	// create WSADATA object
	WSADATA wsaData;
	DWORD u32_Error = ERROR_SUCCESS;
	// socket
	m_hSocket = INVALID_SOCKET;



	// Initialize Winsock
	u32_Error = WSAStartup(MAKEWORD(2, 2), &wsaData);

	return u32_Error;
}

DWORD cClientSocket::CreateSocket()
{
	DWORD u32_Error = ERROR_SUCCESS;

	if ((u32_Error = Initialize()))
	{
		return u32_Error;
	}

	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return WSAGetLastError();
	}

	return u32_Error;

}

DWORD cClientSocket::ConnectTo(DWORD u32_ServerIP, USHORT u16_ServerPort)
{

	DWORD u32_Error = ERROR_SUCCESS;
	
	if (CreateSocket())
	{
		u32_Error = WSAGetLastError();
		cout << "Create socket failed with error " << u32_Error << endl;
		return u32_Error;
	}
	
	sockaddr_in kAddressStruct;
	ZeroMemory(&kAddressStruct, sizeof(kAddressStruct));
	kAddressStruct.sin_family = AF_INET;
	kAddressStruct.sin_port = htons(u16_ServerPort);
	kAddressStruct.sin_addr.s_addr = u32_ServerIP;
	do
	{

		if (connect(m_hSocket, (LPSOCKADDR)&kAddressStruct, sizeof(SOCKADDR_IN)) != 0)
		{
			cout << "Could not connect to server. (" << WSAGetLastError() << ")" << endl;
		}
		else
		{
			cout << "Got connection with server, start working..." << endl;
			return u32_Error;
		}
		Sleep(5000);
	} while (true);


}

DWORD cClientSocket::SendPackage(char* package, int packageSize)
{
	return send(m_hSocket, package, packageSize, 0);
}

DWORD cClientSocket::ReceivePackage(char* package, int packageSize)
{
	return recv(m_hSocket, package, packageSize, 0);
}

#pragma endregion