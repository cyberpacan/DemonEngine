#pragma once

#include <memory>

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

        virtual int Start(unsigned int winWidth, unsigned int winHeight, const char* winTitle);
        void Close();

        virtual void onUpdate() {}


    private:
        std::unique_ptr<class Window> m_pWindow;

	};
}