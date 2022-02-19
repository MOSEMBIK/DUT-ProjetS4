#include <Game/Game.hpp>
#include <Game/Robot.hpp>

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

	glfwMakeContextCurrent(mainWindow->GetWindow());

	if (glewInit() != GLEW_OK)
    {
		cout << "ERROR!" << endl;
		glfwTerminate();
		return false;
    }

    loadRequieredResources();

    glfwSwapInterval(m_vsync);
	glClearColor(0, 0, 0, 1);

	glfwSetKeyCallback(mainWindow->GetWindow(), onKeyPressed);

    mainCamera = new Camera();

    // Hide one useless faces
	//glEnable(GL_CULL_FACE); 

    // Manage depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // Make the lights
	DirectionalLight dirLight(vec3(-1.0f, -1.0f, -1.0f), vec3(0.1f), vec3(1.0f), vec3(1.0f));

	vector<PointLight> pointLights;
	for(int i = 0; i < 4; i++)
		pointLights.push_back(PointLight(i));

    double posX, posY;
	glfwGetCursorPos(mainWindow->GetWindow(), &posX, &posY);
    m_mousePos = vec2(posX, posY);

    map = new Map();
	map->generateMap(13);
	
	Robot* robot = new Robot();
	robot->loadOBJ("assets/models/Bomberman.obj");
	robot->getTransform().SetPosition(vec3(6.0f, 0.0f, 6.0f));
	map->addActor(robot);

	m_lastTime = m_currentTime - 1;

	mainCamera->GetTransform().SetPosition(vec3(-5.86219f, -10.4262f, -20.7321f));
	mainCamera->GetTransform().SetRotation(vec3(0.446024f, 0.00986233f, -0.0049932f));
    return true;
}

bool Game::loadRequieredResources()
{
    static unsigned int textureWhiteID;
	if(!Resource::LoadTexture("assets/white_texture.png", textureWhiteID))
	{
		cout << "Failed to load white texture" << endl;
		glfwTerminate();
		return false;
	}

    static unsigned int textureBlackID;
	if(!Resource::LoadTexture("assets/black_texture.png", textureBlackID))
	{
		cout << "Failed to load black texture" << endl;
		glfwTerminate();
        return false;
	}

    //Load BasicShader
	basicShader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");
	Shader::Register("Base", *basicShader);

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

	processInputs(mainWindow->GetWindow());
	
	m_directionalLight.SendToShader(*basicShader);
	for(PointLight pointLight : m_pointsLights)
		pointLight.SendToShader(*basicShader);

	map->update(m_deltaTime);
	map->draw();

	if(m_currentTime - m_lastTime >= 1)
	{
		std::string fpsCount = "FPS :";
		fpsCount += std::to_string((int)(1 / m_deltaTime));
		glfwSetWindowTitle(mainWindow->GetWindow(), fpsCount.c_str());
		m_lastTime = m_currentTime;
	}

	glfwSwapBuffers(mainWindow->GetWindow());
	glfwPollEvents();
}

void Game::processInputs(GLFWwindow* window)
{
	// Camera movement
	Transform* cameraTransform =  &mainCamera->GetTransform();
    const float cameraSpeed = 5.0f;

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        mainCamera->SetFOV(mainCamera->GetFOV() - 1);
	}

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        mainCamera->SetFOV(mainCamera->GetFOV() + 1);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraTransform->Translate(vec3(0.0f, 0.0f, 1.0f) * cameraTransform->GetRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraTransform->Translate(vec3(0.0f, 0.0f, -1.0f) * cameraTransform->GetRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraTransform->Translate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * cameraSpeed * m_deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraTransform->Translate(vec3(-1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * cameraSpeed * m_deltaTime);
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float deltaX = xpos - m_mousePos.x, deltaY = ypos - m_mousePos.y;

	// Camera rotation
	const float rotationSpeed = 0.007f;
	cameraTransform->Rotate(vec3(0.0f, 1.0f, 0.0f) * cameraTransform->GetRotation() * rotationSpeed * deltaX);
	cameraTransform->Rotate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * rotationSpeed * deltaY);

	m_mousePos = vec2(xpos, ypos);
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}