#include "ClientPCH.h"

#include "Client.h"

int main()
{
	unique_ptr<Client> myClient = std::make_unique<Client>();

	bool success = myClient->Init();

	if (success == true)
	{
		myClient->Run();
	}

	myClient->Shutdown();

	return 0;
}