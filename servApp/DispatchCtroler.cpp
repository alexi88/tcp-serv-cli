#include "DispatchCtroler.h"
#include "Packet.h"

int cDispatchCtroler::m_ClientId = 0;

#pragma region Methods

DWORD cDispatchCtroler::InitHosting(char* serverIP, int serverPort)
{
	DWORD u32_Error = ERROR_SUCCESS;
	m_ServerSocket = new cServerSocket();

	if ((u32_Error=m_ServerSocket->ListenForClients(inet_addr(serverIP), serverPort)))
	{
		return u32_Error;
	}
	
	//thread that waits for new incomming connections
	m_hThreadAcceptClients = CreateThread(0, 0, AcceptClients, this, 0, &u32_Error);
	
	return u32_Error;
}

ULONG WINAPI cDispatchCtroler::AcceptClients(void* p_Param)
{
	cDispatchCtroler* p_This = (cDispatchCtroler*)p_Param;
	p_This->AcceptNewClients();
	return 0;
}

DWORD cDispatchCtroler::AcceptNewClients()
{
	DWORD u32_Error = ERROR_SUCCESS;
	
	while (true)
	{
		u32_Error = m_ServerSocket->AcceptNewClient(m_ClientId);
		//if client connection added successfully start receiving client packets
		if (!u32_Error)
		{
			cout << "Accepted new client connection" << endl;
			//thread that processes incoming packets from current client
			m_hThreadProcessEvents = CreateThread(0, 0, ProcessEvents, this, 0, &u32_Error);
			Sleep(10);
			m_ClientId++;
		}

	}
	return u32_Error;
}


ULONG WINAPI cDispatchCtroler::ProcessEvents(void* p_Param)
{
	cDispatchCtroler* p_This = (cDispatchCtroler*)p_Param;
	p_This->ProcessEvents();
	return 0;
}

DWORD cDispatchCtroler::ProcessEvents()
{
	DWORD u32_Error = ERROR_SUCCESS;
	const int BUFF_SIZE = 10240;
	char buff[BUFF_SIZE];
	int bytesRead = 0;
	ZeroMemory(buff, BUFF_SIZE);
	const int currentClientID = m_ClientId;
	int offset = 0;
	int packetType = 0;
	int packetLen = 0;
	int packetSize = 0;
	SOCKET hClientSocket = m_ServerSocket->GetClientSocket(currentClientID);
	//client connection has been closed
	if (!hClientSocket)
	{
		return u32_Error;
	}
	do{
		//bytesRead = m_ServerSocket->ReceivePackage(buff + offset, BUFF_SIZE - offset);
		bytesRead = recv(hClientSocket, buff + offset, BUFF_SIZE - offset, 0);
		if (bytesRead == SOCKET_ERROR)
		{
			u32_Error = WSAGetLastError();
			cout << "Server socket failed with error " << u32_Error << "Client ID("<<currentClientID<<")"<<endl;
			m_ServerSocket->RemoveClosedSocket(currentClientID);
			return u32_Error;
		}
		packetType = (int)*(buff);
		packetLen = (int)*(buff + sizeof(int));
		packetType = (int)*(buff);
		packetLen = (int)*(buff + sizeof(int));
		packetSize = packetLen/*buffer content size*/ + 2 * sizeof(int)/*type+length*/;
		//check if we received an entire package
		if (bytesRead >= packetSize)
		{
			cPacket* packet = new cPacket(packetType);
			packet->Deserialize((byte*)(buff + 2 * sizeof(int)), packetLen);
			HandlePacket(currentClientID, packet);
			//shift the remaining buff content to the left
			if (bytesRead > packetSize)
			{
				offset = packetSize;
				memmove(buff, buff + offset, bytesRead - offset);
				//continue reading from the current offset
				offset = bytesRead - offset;
			}
			else
			{
				//reset the buffer offset and continue reding from socket
				offset = 0;
			}

			
		}
		//if we didn't receive the entire package continue reading from socket
		else
		{
			offset += bytesRead;
		}



	} while (TRUE);
}



DWORD cDispatchCtroler::HandlePacket(int clientID,cPacket* packet)
{
	DWORD u32_Error = ERROR_SUCCESS;
	string str((char*)packet->GetBuff(),packet->GetLength());
	cout <<"Packet content received from client "<<clientID<<": " <<str<<endl;
	delete packet;

	return u32_Error;
}

DWORD cDispatchCtroler::SendCmd(int clientId, cPacket& packet)
{
	DWORD u32_Error = ERROR_SUCCESS;
	int buffSize = 0;
	unique_ptr<byte[]> sendBuff = packet.Serialize(buffSize);
	u32_Error = m_ServerSocket->SendPackage(clientId, (char*)sendBuff.get(), buffSize);
	return u32_Error;
}

vector<int> cDispatchCtroler::GetClientsIds()
{
	return m_ServerSocket->GetClientsIds();
}

#pragma endregion