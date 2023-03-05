#pragma once

#include <iostream>
#include <memory>

using namespace std;

namespace DemonEngine
{
	class Application
	{
	public:
        Application();
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        virtual int start(unsigned int windowWidth, unsigned int windowHeight, const char* title);
          
        void Close();

        virtual void onUpdate() {}


    private:
        std::unique_ptr<class Window> mPWindow;

	};
}