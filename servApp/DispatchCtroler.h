#include "ServerSocket.h"
#include "Packet.h"

class cDispatchCtroler
{
	cServerSocket* m_ServerSocket;
	HANDLE m_hThreadAcceptClients;
	HANDLE m_hThreadProcessEvents;

	//keep track of client id;
	static int m_ClientId;
public:	
	#pragma region C-tors/D-tors

	cDispatchCtroler(){};
	~cDispatchCtroler(){};

	#pragma endregion

	#pragma region Methods

	DWORD InitHosting(char* serverIP, int serverPort);
	DWORD SendCmd(int clientId, cPacket& packet);
	DWORD ProcessEvents();
	
	DWORD HandlePacket(int clientID, cPacket* packet);
	vector<int> GetClientsIds();
	/*** Thread specific action EntryPoints ****/
	DWORD AcceptNewClients();
	static ULONG WINAPI AcceptClients(void* p_Param);
	static ULONG WINAPI ProcessEvents(void* p_Param);
	
	#pragma endregion
};