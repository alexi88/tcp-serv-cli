#include "ServerSocket.h"

DWORD cServerSocket::Initialize()
{
	// create WSADATA object
	WSADATA wsaData;
	DWORD u32_Error = ERROR_SUCCESS;
	// socket
	m_hServerSocket = INVALID_SOCKET;
	
	// Initialize Winsock
	u32_Error = WSAStartup(MAKEWORD(2, 2), &wsaData);

	//initialize client list mutex to control access to clients list 
	hClientListMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hClientListMutex)
	{
		u32_Error = GetLastError();
		cout << "Failed to create client list mutex " << u32_Error;
		return u32_Error;
	}
	return u32_Error;
}

DWORD cServerSocket::CreateSocket()
{
	DWORD u32_Error = ERROR_SUCCESS;

	if ((u32_Error = Initialize()))
	{
		return u32_Error;
	}

	m_hServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hServerSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return WSAGetLastError();
	}

	return u32_Error;
}

SOCKET cServerSocket::GetClientSocket(int clientId)
{
	SOCKET hSocket = NULL;
	WaitForSingleObject(hClientListMutex, INFINITE);
	map<int, SOCKET>::iterator it;
	if ((it=m_ClientsList.find(clientId)) != m_ClientsList.end())
	{
		hSocket = it->second;
	}
	ReleaseMutex(hClientListMutex);
	return  hSocket ;
}

vector<int> cServerSocket::GetClientsIds()
{
	vector<int> clientIds;

	// go through all clients
	std::map<int, SOCKET>::iterator iter;
	WaitForSingleObject(hClientListMutex, INFINITE);
	
	for (iter = m_ClientsList.begin(); iter != m_ClientsList.end(); iter++)
		clientIds.push_back(iter->first);
	ReleaseMutex(hClientListMutex);
	return clientIds;

}
void  cServerSocket::RemoveClosedSocket(int clienId)
{
	WaitForSingleObject(hClientListMutex, INFINITE);
	m_ClientsList.erase(clienId);
	ReleaseMutex(hClientListMutex);
	
}


DWORD  cServerSocket::ListenForClients(DWORD u32_ServerIP, USHORT u16_ServerPort)
{
	DWORD u32_Error = ERROR_SUCCESS;

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(u16_ServerPort);
	addr.sin_addr.S_un.S_addr = u32_ServerIP;

	if ((u32_Error=CreateSocket()))
	{
		cout << "Create socket failed with error: " << u32_Error << endl;
		return u32_Error;
	}

	if (bind(m_hServerSocket,(LPSOCKADDR)&addr,sizeof(SOCKADDR_IN))==SOCKET_ERROR)
	{
		u32_Error = WSAGetLastError();
		cout << "Bind failed with error: " << u32_Error << endl;
		closesocket(m_hServerSocket);
		WSACleanup();
		return u32_Error;
	}

	if (listen(m_hServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		u32_Error = WSAGetLastError();
		cout << "Listen failed with error: " << u32_Error <<endl;
		closesocket(m_hServerSocket);
		WSACleanup();
		return u32_Error;
	}
	

	return u32_Error;
}

DWORD cServerSocket::AcceptNewClient(int id)
{
	DWORD u32_Error = ERROR_SUCCESS;
	m_hClientSocket = accept(m_hServerSocket, NULL, NULL);

	if (m_hClientSocket != INVALID_SOCKET)
	{
		WaitForSingleObject(hClientListMutex, INFINITE);
		m_ClientsList.insert(pair<int, SOCKET>(id, m_hClientSocket));
		ReleaseMutex(hClientListMutex);
		
	}
	return u32_Error;
}

DWORD cServerSocket::SendPackage(int clientId,char* buff,int buffSize)
{
	SOCKET hSocket = GetClientSocket(clientId);
	if (!hSocket)
		return NULL;
	return send(hSocket, buff, buffSize, 0);
}

DWORD cServerSocket::ReceivePackage(char* package, int packageSize)
{
	return recv(m_hServerSocket, package, packageSize, 0);
}