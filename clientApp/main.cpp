#include "DispatchCtroler.h"

using namespace std;

int main(char* argc,char** argv)
{
	DWORD u32_Error = ERROR_SUCCESS;
	cout << "This is my client APP"<<endl;

	cDispatchCtroler* dispatch = new cDispatchCtroler();
	do
	{

		if ((u32_Error = dispatch->InitConnection("127.0.0.1", 1960)))
		{
			cout << "Error while trying to connect to server..." << endl;
			return u32_Error;
		}

		dispatch->ProcessEvents();
	} while (true);
	return u32_Error;
}