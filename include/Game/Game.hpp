#pragma once

#include <utils.h>

#include <Game/Map.hpp>

#include <Engine/Camera.hpp>
#include <Engine/Lights.hpp>
#include <Engine/Window.hpp>

#include <glm/vec2.hpp>

enum VSync { OFF = 0, ONE_FRAME = 1, TWO_FRAME = 2};
enum GameState { MAIN_MENU, GAME };

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

class Game {
private:
    Game();

    float m_lastTime;
    float m_currentTime;
    float m_deltaTime;

    glm::vec2 m_mousePos;

    DirectionalLight m_directionalLight;
    std::vector<PointLight> m_pointsLights;

    Window* mainWindow;
    Camera* mainCamera;
    Map* map;
    Shader* basicShader;

    GameState m_gameState;
    VSync m_vsync;

    // Singleton
    static Game* m_instance;

    bool init();
    bool loadRequieredResources();
    void processInputs(GLFWwindow* window);

public:
    // Suppression du clonage et de l'opérateur =
    Game(Game&) = delete;
    void operator= (const Game&) = delete;

    ~Game();

    void update();

    /**
     * @brief Get the Singleton instance of Game
     * 
     * @return Game* 
     */
    static Game* getInstance();

    /**
     * @brief Get the Directional Light
     * 
     * @return DirectionalLight* 
     */
    inline DirectionalLight* getDirectionalLight() { return &m_directionalLight; }

    /**
     * @brief Get the Point Light by index
     * 
     * @param i 
     * @return PointLight* 
     */
    inline PointLight* getPointLight(unsigned int i) { if(i < m_pointsLights.size()) return &m_pointsLights[i]; return nullptr;}

    /**
     * @brief Get the Main Window
     * 
     * @return Window* 
     */
    inline Window* getMainWindow() { return mainWindow; }

    /**
     * @brief Get the Main Camera
     * 
     * @return Camera* 
     */
    inline Camera* getMainCamera() { return mainCamera; }

    /**
     * @brief Get the Map
     * 
     * @return Map* 
     */
    inline Map* getMap() { return map; }

    /**
     * @brief Get the Mouse Position
     * 
     * @return glm::vec2 
     */
    inline glm::vec2 getMousePosition() { return m_mousePos; }

    /**
     * @brief Get the time elapsed since the start of the software
     * 
     * @return float 
     */
    inline float getCurrentTime() { return m_currentTime; }

    /**
     * @brief Get the time between last frame and the current one
     * 
     * @return float 
     */
    inline float getDeltaTime() { return m_deltaTime; }

    /**
     * @brief Get the VSync setting
     * 
     * @return VSync 
     */
    inline VSync getVSync() { return m_vsync; }

    /**
     * @brief Set the VSync setting
     * 
     * @param v 
     * @return VSync 
     */
    inline void setVSync(VSync v) { m_vsync = v; }

    /**
     * @brief Get the current state of the game
     * 
     * @return GameState 
     */
    inline GameState getState() { return m_gameState; }

	/**
	 * @brief Set a new state of the game
	 * 
	 * @param state 
	 */
    void setState(GameState state);
};
