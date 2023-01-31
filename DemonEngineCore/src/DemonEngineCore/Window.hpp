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
        unsigned int get_width() const { return m_data.width; }
        unsigned int get_height() const { return m_data.height; }


        void setEventCallback(const EventCallback& callback)
        {
            m_data.eventCallback = callback;
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

        GLFWwindow* m_pWindow = nullptr;
        WindowData m_data;
        float m_bgc[4] = { 0.33f, 0.33f, 0.33f, 0.f };
    };

}