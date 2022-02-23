#pragma once

#include <Engine/Window.hpp>

#include <glm/vec2.hpp>

class Widget {
protected:
    glm::vec2 m_position;
    glm::vec2 m_anchor;
    Window* m_window;

public:
    Widget(Window* window, glm::vec2 position, glm::vec2 anchor);
    
    inline void setPosition(glm::vec2 position) { m_position = position; }
    
    inline glm::vec2 getPosition() const { return m_position; }

    inline void setAnchor(glm::vec2 anchor) { m_anchor = anchor; }
    
    inline glm::vec2 getAnchor() const { return m_anchor; }
    
    virtual void draw() = 0;
};
