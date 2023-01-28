#include <iostream>
#include <memory>

#include <DemonEngineCore/Application.hpp>

class MyApp : public DemonEngine::Application
{
    virtual void onUpdate() override
    {
        //    std::cout << "Update frame: " << frame++ << std::endl;
    }

    //int frame = 0;
};


int main()
{
    auto myApp = std::make_unique<MyApp>();

    int returnCode = myApp->start(1024, 760, "My first App");

    //std::cin.get();

    return returnCode;
}