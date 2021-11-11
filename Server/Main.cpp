#include "ServerPCH.h"

#include "Server.h"

int main()
{
	unique_ptr<Server> myServer = std::make_unique<Server>();

	bool success = myServer->Init();

	if (success == true)
	{
		myServer->Run();
	}

	myServer->Shutdown();

	return 0;

}