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

    mainCamera = new Camera();

    // Hide one useless faces
	//glEnable(GL_CULL_FACE); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // Manage depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // Make the lights
	m_directionalLight = DirectionalLight(vec3(+1.0f, -0.75f, -1.5f), vec3(0.1f), vec3(1.0f), vec3(1.0f));

	for(int i = 0; i < 4; i++)
		m_pointsLights.push_back(PointLight(i));

    double posX, posY;
	glfwGetCursorPos(mainWindow->getWindow(), &posX, &posY);
    m_mousePos = vec2(posX, posY);

    map = new Map();
	map->generateMap(13);
	
	// Test de robots
	for (int i = 0; i < 10; i++) {
		Robot* robot = new Robot(map);
		Bomb* bomb = new Bomb(map, vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f), 5, 5);
		robot->getTransform().setPosition(vec3(6.0f, 0.0f, 6.0f));
		bomb->getTransform().setPosition(vec3(rand()%11+1, 0, rand()%11+1));
		map->addActor(robot);
		map->addActor(bomb);
	}

	m_lastTime = m_currentTime - 1;

	mainCamera->getTransform().setPosition(vec3(-5.86219f, -10.4262f, -20.7321f));
	mainCamera->getTransform().setRotation(vec3(0.45f, 0.0f, 0.0f));
    return true;
}

Button* button;

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
	button = new Button(mainWindow, vec2(0, 25), vec2(0.5f, 0.0f), vec2(475, 75), "assets/button.png");

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


void Game::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_deltaTime = glfwGetTime() - m_currentTime;
	m_currentTime = glfwGetTime();

	processInputs(mainWindow->getWindow());
	
	m_directionalLight.sendToShader(*basicShader);
	for(PointLight pointLight : m_pointsLights)
		pointLight.sendToShader(*basicShader);

	map->update(m_deltaTime);
	map->draw();

	// Test de bouton
	button->draw();
	Label label(mainWindow, vec2(0, 62.5), vec2(0.5f, 0.0f), "Test Bouton", "assets/fonts/bomberman.ttf", ALIGN_CENTER | ALIGN_MIDDLE);
	label.setFontColor(vec3(1.0f, 1.0f, 1.0f));
	label.draw();

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
	float deltaX = xpos - m_mousePos.x, deltaY = ypos - m_mousePos.y;

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