#include "ClientSocket.h"
#include "Packet.h"

using namespace std;
class cDispatchCtroler
{
	cClientSocket* m_ClientSocket;
public:
	#pragma region C-tors/D-tors

	cDispatchCtroler(){};
	~cDispatchCtroler(){};

	#pragma endregion

	#pragma region Methods

	DWORD InitConnection(char* serverIP,int serverPort);
	DWORD ProcessEvents();
	DWORD HandlePacket(cPacket* packet);
	DWORD SendCmd(cPacket& packet);

	#pragma endregion
};
