//#include <iostream>
#include <Windows.h>
#include <memory>
#include <DemonEngineCore/Application.hpp>

using namespace std;
using namespace DemonEngine;



class MyApp : public Application
{

	virtual void onUpdate() override
	{
		//cout << "Frame updating: " << ++frame << endl;
	}

	int frame = 0;
};

int main()
{
	
	
	auto myApp = make_unique<MyApp>();
	int returnCode = myApp->Start(1024, 1024, "DemonEngine Editor");
	return returnCode;
	//std::cin.get();
}