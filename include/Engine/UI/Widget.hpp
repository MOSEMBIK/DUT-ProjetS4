#pragma once

#include <glm/vec2.hpp>

class Widget {
protected:
    glm::vec2 m_position;

public:
    Widget(glm::vec2 position);
    
    inline void setPosition(glm::vec2 position) { m_position = position; }
    
    inline glm::vec2 getPosition() const { return m_position; }
    
    virtual void draw() = 0;
};
