#pragma once

#include <sstream>

#include <Engine/Event/Event.hpp>

class Window;

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}

    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
    AppUpdateEvent(const Window* window, float deltaTime) : m_Window(window), m_DeltaTime(deltaTime) {}

    inline float GetDeltaTime() const { return m_DeltaTime; }
    inline const Window* GetWindow() const { return m_Window; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "AppUpdateEvent: " << m_DeltaTime << "ms";
        return ss.str();
    }

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
    const Window* m_Window;
    float m_DeltaTime;    
};