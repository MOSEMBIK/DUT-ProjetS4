#pragma once

#include <utils.h>

#include <Game/Map.hpp>

#include <Engine/Camera.hpp>
#include <Engine/Lights.hpp>
#include <Engine/Window.hpp>

#include <Engine/Event/Event.hpp>
#include <Engine/Event/MouseEvent.hpp>
#include <Engine/Event/KeyEvent.hpp>
#include <Engine/Event/ApplicationEvent.hpp>

#include <Multiplayer/Server.hpp>
#include <Multiplayer/Client.hpp>

#include <glm/vec2.hpp>

enum GameState {
	MAIN_MENU, OPTIONS,
	SOLO_MENU, SOLO_GAME, SOLO_LOADING, SOLO_PAUSED,
	MULTI_MENU, MULTI_CREATE_SERVER, MULTI_LOADING_SERVER, MULTI_JOIN_SERVER, MULTI_GAME_CLIENT, MULTI_GAME_SERVER
};

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

class Game {
private:
    Game();

    float m_lastTime;
    float m_currentTime;
    float m_deltaTime;

    bool m_running = true;
	unsigned int keyPressed = 0;

    DirectionalLight m_directionalLight;
    std::vector<PointLight> m_pointsLights;

    Window* mainWindow;
    Camera* mainCamera;
    Map* map;
    Shader* basicShader;

    GameState m_gameState;

    // Options
	std::string m_username;
	bool m_fullscreen;
	glm::ivec2 m_windowSize;


    // Singleton
    static Game* m_instance;

	// Multiplayer
	Server* m_server;
	Client* m_client;
	std::string m_mapInfo;
	std::string m_updatePosRot;


    bool init();
    bool loadRequiredResources();
    void processInputs(GLFWwindow* window);

public:
	std::vector<unsigned char> m_gameSettings = {13, 4, 90, 10, 3};
    // Suppression du clonage et de l'opérateur =
    Game(Game&) = delete;
    void operator= (const Game&) = delete;

    ~Game();

	bool postInit();
	bool updateWindowOptions();
    void onEvent(Event& e);
    bool onUpdate(AppUpdateEvent& e);
    bool onClose(WindowCloseEvent& e);
    void run();

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

	/**
	 * @brief update map data to load
	 * 
	 * @param data 
	 */
	inline void loadMap(const std::string& data) { m_mapInfo = data; }

	/**
	 * @brief update map data for players
	 * 
	 * @param data 
	 */
	inline void updatePosRot(const std::string& data) { m_updatePosRot = data; }
};
