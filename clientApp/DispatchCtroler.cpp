#include "DispatchCtroler.h"

DWORD cDispatchCtroler::InitConnection(char* serverIP, int serverPort)
{
	DWORD u32_Error = ERROR_SUCCESS;

	m_ClientSocket = new cClientSocket();

	u32_Error = m_ClientSocket->ConnectTo(inet_addr(serverIP), serverPort);

	if (u32_Error)
	{
		cout << "Failed to connect to the server with error code: " << u32_Error << endl;
	}

	return u32_Error;
}


DWORD cDispatchCtroler::ProcessEvents()
{
	DWORD u32_Error = ERROR_SUCCESS;
	const int BUFF_SIZE = 10240;
	char* buff = new char[BUFF_SIZE];
	int bytesRead = 0;
	ZeroMemory(buff, BUFF_SIZE);

	int offset = 0;
	int packetType = 0;
	int packetLen = 0;
	int packetSize = 0;
	do{
		bytesRead = m_ClientSocket->ReceivePackage(buff + offset, BUFF_SIZE - offset);
		if (bytesRead == SOCKET_ERROR)
		{
			u32_Error = WSAGetLastError();
			cout << "Server socket failed with error " << u32_Error << endl;
			return u32_Error;
		}
		packetType = (int)*(buff);
		packetLen = (int)*(buff + sizeof(int));
		packetSize = packetLen/*buffer content size*/ + 2 * sizeof(int)/*type+length*/;
		//check if we received an entire package
		if (bytesRead >= packetSize)
		{
			cPacket* packet = new cPacket(packetType);
			packet->Deserialize((byte*)(buff + 2 * sizeof(int)),packetLen);
			//shift the remaining buff content to the left
			if (bytesRead > packetSize)
			{
				offset =packetSize;
				memmove(buff, buff + offset, bytesRead - offset);
				//continue reading from the current offset
				offset = bytesRead - offset;
			}
			else
			{
				//reset the buffer offset and continue reding from socket
				offset = 0;
			}

			HandlePacket(packet);
		}
		//if we didn't receive the entire package continue reading from socket
		else
		{
			offset += bytesRead;
		}



	} while (TRUE);
}

DWORD cDispatchCtroler::HandlePacket(cPacket* packet)
{
	DWORD u32_Error = ERROR_SUCCESS;


	switch (packet->GetPacketType())
	{
	case INIT_CONNECTION_CMD:
		{
			string str((char*)packet->GetBuff(), packet->GetLength());
			cout << "Packet content received: " << str << endl;
			char* response = "echo message from client";
			
			unique_ptr<cPacket> respPacket(new cPacket(INIT_CONNECTION_CMD));
			respPacket->Deserialize((byte*)response, strlen(response));

			SendCmd(*respPacket);
		}
		break;

	default:
		break;
	}
	delete packet;

	return u32_Error;
}

DWORD cDispatchCtroler::SendCmd(cPacket& packet)
{
	DWORD u32_Error = ERROR_SUCCESS;
	int buffSize = 0;
	unique_ptr<byte[]> sendBuff = packet.Serialize(buffSize);
	m_ClientSocket->SendPackage((char*)sendBuff.get(), buffSize);

	return u32_Error;
}
