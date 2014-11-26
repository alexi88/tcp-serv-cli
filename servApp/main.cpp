#include<iostream>
#include "DispatchCtroler.h"
using namespace std;
void main()
{
	cout << "This is my server APP" << endl;

	cDispatchCtroler* dispatch = new cDispatchCtroler();

	dispatch->InitHosting("127.0.0.1", 1960);


	//send commands to client
	
	//	Sleep(20000);
		char * cmd = "this is my cmd";
		//cPacket* myCmd = new cPacket(INIT_CONNECTION_CMD);
		unique_ptr<cPacket> myCmd(new cPacket(INIT_CONNECTION_CMD));
		myCmd->Deserialize((byte*)cmd, strlen(cmd));
		vector<int> ids;
		while (true)
		{
			ids = dispatch->GetClientsIds();
			for (vector<int>::iterator it = ids.begin(); it != ids.end(); it++)
			{
				dispatch->SendCmd(*it, *myCmd);
			}
			//Sleep(5000);
		}
	

}