#pragma once

#include <Engine/UI/Widget.hpp>
#include <Engine/UI/Label.hpp>

#include <Engine/Shader.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Event/EventReceiver.hpp>

#include <glm/vec3.hpp>

#include <functional>

class Button : public Widget, public IEventReceiver {
    enum State { NONE, HOVERED};

protected:
    static Mesh* buttonQuad;
    static Shader* buttonShader;
    static Shader* buttonSlicedShader;

    unsigned int m_nineSlice = 16;

    bool m_clicked;

    State m_state;
	Label m_label;

    glm::vec2 m_size;

    Texture* m_texture;
    glm::vec3 m_color;
    Texture* m_clickedTexture;
    glm::vec3 m_clickedColor;
    Texture* m_highlightedTexture;
    glm::vec3 m_highlightedColor;

    std::function<void()> onClick = []() {};

public:
    Button(Window* window, glm::vec2 position, glm::vec2 anchor, glm::vec2 size, char* texture, char* clickedTexture, char* highlightedTexture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    Button(Window* window, glm::vec2 position, glm::vec2 anchor, glm::vec2 size, char* texture, char* clickedTexture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    Button(Window* window, glm::vec2 position, glm::vec2 anchor, glm::vec2 size, char* texture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    ~Button();

    inline void setSize(glm::vec2 size) { m_size = size; }
    inline void setLabel(Label label) { m_label = label; }

    inline void setTexture(Texture* texture) { m_texture = texture; }
    inline void setClickedTexture(Texture* texture) { m_clickedTexture = texture; }
    inline void setHighlightedTexture(Texture* texture) { m_highlightedTexture = texture; }

    inline void setNineSlice(int sliceSize) { m_nineSlice = sliceSize; }

    inline void setColor(glm::vec3 color) { m_color = color; } 
    inline void setClickedColor(glm::vec3 color) { m_clickedColor = color; } 
    inline void setHighlightedColor(glm::vec3 color) { m_highlightedColor = color; } 

    inline glm::vec2 getSize() { return m_size; } 
    inline Label& getLabel() { return m_label; } 

    inline Texture* getTexture() const { return m_texture; }
    inline Texture* getClickedTexture() const { return m_clickedTexture; }
    inline Texture* getHighlightedTexture() const { return m_highlightedTexture; }

    inline glm::vec3 getColor() const { return m_color; }
    inline glm::vec3 getClickedColor() const { return m_clickedColor; }
    inline glm::vec3 getHighlightedColor() const { return m_highlightedColor; }

    inline void setOnClickCallback(std::function<void()> func) { onClick = func; }

    virtual void onEvent(Event& event);

    void draw();
    
	bool onMouseMoved(MouseMovedEvent& e);
	bool onMouseButtonPressed(MouseButtonPressedEvent& e);
	bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
};
