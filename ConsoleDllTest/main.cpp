#include <iostream>

#include "ComLibWrapper.h"

using namespace std;


int main(int argc, const char * argv[]) {
	auto * wo = CreateComLibWrapper();
	
	wo->SetStatusCallback([](ComLibConnectionStatus status) {
		std::cout << "From callback: " << (int)status << std::endl;
		
	});
	

	

	std::cout << "Please enter 1 to start support or 2 to stop support!\n";
	int val;

	
	while (true)
	{
		cin >> val;
		if (val == 1)
		{
			/*if success is false it means that the RemoteService is down or that the RemoteService can't start connectwise service.
		   * either way the SDK consumer can't resolve the problem and logs have to be sent.
		   *
		   * */
			try
			{
				auto succeed = wo->RequestSupport();
				if (!succeed) cout << "false";
				else cout<< "true";
			}
			catch (const std::exception&)
			{

			}
			catch (...)
			{

			}
		
		}
		else if (val == 2)
		{
			/*if success is false it means that the RemoteService is down or that the RemoteService can't start connectwise service.
		   * either way the SDK consumer can't resolve the problem and logs have to be sent.
		   *
		   * */
			try
			{
				auto succeed = wo->StopSupport();
				if (!succeed) cout << "false";
				else cout << "true";
			}
			catch (...)
			{

			}
		}
		else if (val == 5)
		{
			break;
		}
		else
		{
			std::cout << "Wrong selected value";
		}
	}
	DestroyComLibWrapper(wo);
	wo = nullptr;
}