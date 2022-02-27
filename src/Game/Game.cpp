#include <Game/Game.hpp>
#include <Game/Robot.hpp>

#include <Engine/UI/Button.hpp>
#include <Engine/UI/Label.hpp>
#include <Engine/Transform.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>

#include <iostream>

using namespace glm;
using namespace std;

vector<Button*> buttons;

Game* Game::m_instance = nullptr;

Game::Game() : m_currentTime(0), m_deltaTime(0), m_mousePos(vec2(0.0f)), m_directionalLight(vec3(-1.0f, -1.0f, -1.0f), vec3(0.1f), vec3(1.0f), vec3(1.0f)), m_gameState(GameState::MAIN_MENU), m_vsync(VSync::ONE_FRAME)
{
    m_currentTime = glfwGetTime();
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    init();
}

bool Game::init()
{
	mainWindow = new Window();

	if (!mainWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mainWindow->getWindow());

	if (glewInit() != GLEW_OK)
    {
		cout << "ERROR!" << endl;
		glfwTerminate();
		return false;
    }

    loadRequieredResources();

    glfwSwapInterval(m_vsync);
	glClearColor(0, 0, 0, 1);

	glfwSetKeyCallback(mainWindow->getWindow(), onKeyPressed);

    // Hide one useless faces
	//glEnable(GL_CULL_FACE); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // Manage depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Make the lights
	m_directionalLight = DirectionalLight(vec3(+1.0f, -0.75f, -1.5f), vec3(0.1f), vec3(1.0f), vec3(1.0f));

	for (int i = 0; i < 4; i++)
		m_pointsLights.push_back(PointLight(i));

	double posX, posY;
	glfwGetCursorPos(mainWindow->getWindow(), &posX, &posY);
	m_mousePos = vec2(posX, posY);

	m_lastTime = m_currentTime - 1;

	return true;
}

bool Game::loadRequieredResources()
{
	if(!Resource::loadTexture("assets/white_texture.png", Textures::whiteTexture))
	{
		cout << "Failed to load white texture" << endl;
		glfwTerminate();
		return false;
	}

	if(!Resource::loadTexture("assets/black_texture.png", Textures::blackTexture))
	{
		cout << "Failed to load black texture" << endl;
		glfwTerminate();
        return false;
	}

    //Load BasicShader
	basicShader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");
	Shader::save("Base", basicShader);

	//Load Buttons
	Button *button = new Button(mainWindow, vec2(0, 25), vec2(0.5f, 0.0f), vec2(475, 75), (char *)"assets/button.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f));
	button->setNineSlice(true);
	Label label(mainWindow, vec2(0, 62.5), vec2(0.5f, 0.0f), "Lancer la partie", (char *)"assets/fonts/bomberman.ttf", ALIGN_CENTER | ALIGN_MIDDLE);
	button->setOnClickCallback([]() {
		Game* game = Game::getInstance();
		switch(game->getState())
		{
			case GameState::MAIN_MENU:
				game->setState(GameState::GAME);
				break;
			case GameState::GAME:
				game->setState(GameState::MAIN_MENU);
				break;
		}
	});
	button->setLabel(label);
	buttons.push_back(button);

	this->setState(GameState::MAIN_MENU);

    return true;
}

Game* Game::getInstance() 
{
    if(m_instance == nullptr)
    {
		cout << "New instance" << endl;
        m_instance = new Game();
    }
    return m_instance;
}

Game::~Game()
{
    if(mainWindow != NULL)
        delete mainWindow;

    if(mainCamera != NULL)
        delete mainCamera;
}

void Game::setState(GameState state)
{
	switch (state)
	{
	/**
	 * @brief Load the main menu
	 */
	case GameState::MAIN_MENU:
		if (m_gameState == GameState::GAME) {
			// Delete game content
			delete map;
			delete mainCamera;
			map = nullptr;
			mainCamera = nullptr;
		}
		buttons[0]->getLabel().setText("Lancer la partie");

		cout << "Load main menu" << endl;
		break;
	
	/**
	 * @brief Launch the game
	 */
	case GameState::GAME:
		if (m_gameState == GameState::MAIN_MENU) {
			// Delete main menu content
		}
		buttons[0]->getLabel().setText("Quitter la partie");

		mainCamera = new Camera();
		map = new Map();
		map->generateMap(13);
		
		// Test de robots
		for (int i=0; i < 10; i++) {
			Robot* robot = new Robot(map);
			Bomb* bomb = new Bomb(map, vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f), 5, 5);
			robot->getTransform().setPosition(vec3(6.0f, 0.0f, 6.0f));
			bomb->getTransform().setPosition(vec3(rand()%11+1, 0, rand()%11+1));
			map->addActor(robot);
			map->addActor(bomb);
		}
		
		mainCamera->getTransform().setPosition(vec3(-6.0f, -8.0f, -20.0f));
		mainCamera->getTransform().setRotation(vec3(0.60f, 0.0f, 0.0f));

		cout << "Load game" << endl;
		break;
	}
	
	m_gameState = state;
}


void Game::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_deltaTime = glfwGetTime() - m_currentTime;
	m_currentTime = glfwGetTime();

	processInputs(mainWindow->getWindow());

	switch (m_gameState)
	{
		case GameState::GAME:
			m_directionalLight.sendToShader(*basicShader);
			for(PointLight pointLight : m_pointsLights) {
				pointLight.sendToShader(*basicShader);
			}
			map->update(m_deltaTime);
			map->draw();
			buttons[0]->draw();
			break;

		case GameState::MAIN_MENU:
			buttons[0]->draw();
			break;
	}

	if(m_currentTime - m_lastTime >= 1)
	{
		std::string fpsCount = "FPS :";
		fpsCount += std::to_string((int)(1 / m_deltaTime));
		glfwSetWindowTitle(mainWindow->getWindow(), fpsCount.c_str());
		m_lastTime = m_currentTime;
	}

	glfwSwapBuffers(mainWindow->getWindow());
	glfwPollEvents();
}

void Game::processInputs(GLFWwindow* window)
{
	mainWindow->update();

	if(mainCamera == nullptr)
		return;

	// Camera movement
	Transform* cameraTransform =  &mainCamera->getTransform();
    const float cameraSpeed = 5.0f;

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        mainCamera->setFOV(mainCamera->getFOV() - 1);
	}

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        mainCamera->setFOV(mainCamera->getFOV() + 1);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraTransform->translate(vec3(0.0f, 0.0f, 1.0f) * cameraTransform->getRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraTransform->translate(vec3(0.0f, 0.0f, -1.0f) * cameraTransform->getRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraTransform->translate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->getRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraTransform->translate(vec3(-1.0f, 0.0f, 0.0f) * cameraTransform->getRotation() * cameraSpeed * m_deltaTime);
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float deltaX = xpos - m_mousePos.x,
	deltaY = ypos - m_mousePos.y;
	UNUSED(deltaX);
	UNUSED(deltaY);

	// Camera rotation
	const float rotationSpeed = 0.007f;
	cameraTransform->rotate(vec3(0.0f, 1.0f, 0.0f) * cameraTransform->getRotation() * rotationSpeed * deltaX);
	cameraTransform->rotate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->getRotation() * rotationSpeed * deltaY);

	m_mousePos = vec2(xpos, ypos);
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}