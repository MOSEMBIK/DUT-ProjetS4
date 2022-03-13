#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <list>

#include <glm/vec2.hpp>

#include <Engine/Event/Event.hpp>
#include <Engine/Event/EventReceiver.hpp>

#define DEFAULT_WINDOW_W 1280
#define DEFAULT_WINDOW_H 720

struct WindowProps
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& title = "Bomberman",
                unsigned int width = DEFAULT_WINDOW_W,
                unsigned int height = DEFAULT_WINDOW_H)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Window
{
    public:
        using EventCallbackFn = std::function<void(Event&)>;

    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            unsigned int CursorX, CursorY;
            float Time;
            bool VSync;

            EventCallbackFn EventCallback;
        };

    private:
		GLFWwindow* m_window;
		WindowData m_data;
        std::list<IEventReceiver*> m_EventReceivers;

    public:

    public:
        Window(const WindowProps& props);    
		~Window();

		void onUpdate();
        void onEvent(Event& e)
        {
            for(auto er : m_EventReceivers)
            {
                er->onEvent(e);
                if(e.Handled)
                    break;
            }
        }

		unsigned int getWidth() const { return m_data.Width; }
		unsigned int getHeight() const { return m_data.Height; }

        GLFWwindow* getWindow() const { return m_window; }

		void setEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const;

        void attachEventReceiver(IEventReceiver& er) 
        { 
            m_EventReceivers.push_back(&er); 
            er.onAttached();
        };

        void detachEventReceiver(IEventReceiver& er) 
        { 
            m_EventReceivers.remove(&er); 
            er.onDetached();
        };

        void setTitle(std::string title);

        glm::vec2 getScale() { return glm::vec2( DEFAULT_WINDOW_W / m_data.Width, DEFAULT_WINDOW_H / m_data.Height );}

		static Window* create(const WindowProps& props = WindowProps());

	private:
		void Init(const WindowProps& props);
		void Shutdown();
};
