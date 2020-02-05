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
		 wo->RequestSupport();
		
		}
		else if (val == 2)
		{
			 wo->StopSupport();
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