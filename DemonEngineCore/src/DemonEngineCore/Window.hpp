#pragma once

#include "DemonEngineCore/Event.hpp"

#include <string>
#include <functional>

struct GLFWwindow;

namespace DemonEngine {

    class Window
    {
    public:

        using EventCallback = std::function<void(Event&)>;

        Window(std::string title, const unsigned int width, const unsigned int height);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        void onUpdate();
        unsigned int getWidth() const { return mData.width; }
        unsigned int getHeight() const { return mData.height; }


        void setEventCallback(const EventCallback& callback)
        {
            mData.eventCallback = callback;
        }

    private:

        struct WindowData
        {
            std::string title;
            unsigned int width;
            unsigned int height;
            EventCallback eventCallback;
        };

        int init();
        void shutdown();

        GLFWwindow* mPWindow = nullptr;
        WindowData mData;
        float mBGC[4] = { 0.33f, 0.33f, 0.33f, 0.f };
    };

}