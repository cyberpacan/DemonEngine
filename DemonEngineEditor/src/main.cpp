#include <iostream>
#include <memory>

#include <DemonEngineCore/Application.hpp>

class MyApp : public DemonEngine::Application
{
    virtual void onUpdate() override
    {
        
    }

    
};


int main()
{
    auto myApp = std::make_unique<MyApp>();

    int returnCode = myApp->start(1024, 760, "render");

    

    return returnCode;
}