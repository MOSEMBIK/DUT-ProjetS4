#include <Game/Game.hpp>
#include <Game/Robot.hpp>

#include <Engine/UI/Button.hpp>
#include <Engine/UI/Image.hpp>
#include <Engine/UI/Label.hpp>
#include <Engine/Transform.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>

#include <iostream>

using namespace glm;
using namespace std;
using std::cerr;
using std::endl;

vector<Button*> buttons;
vector<Label*> labels;
vector<Image*> images;
Actor* actor;

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
	mainCamera = new Camera();

	if (!mainWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mainWindow->getWindow());

	if (glewInit() != GLEW_OK)
    {
		cerr << "ERROR!" << endl;
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
	m_directionalLight.sendToShader(*basicShader);
	for (PointLight pointLight : m_pointsLights)
		pointLight.sendToShader(*basicShader);

	double posX, posY;
	glfwGetCursorPos(mainWindow->getWindow(), &posX, &posY);
	m_mousePos = vec2(posX, posY);

	m_lastTime = m_currentTime - 1;

	cerr << "Game initialized" << endl;
	return true;
}

bool Game::loadRequieredResources()
{
	cerr << "Loading resources..." << endl;
	float time = glfwGetTime();
	std::vector<string> assets = {
		"white_texture.png", "black_texture.png", "home_background.png",
		"space_background.png", "bomberboy_1.png", "bomberboy_2.png",
		"blue_rectangle.png"
	};
	std::vector<Texture*> textures = {
		&Textures::whiteTexture, &Textures::blackTexture, &Textures::homeBackground,
		&Textures::spaceBackground, &Textures::bomberboy1, &Textures::bomberboy2,
		&Textures::blueRectangle
	};
	for (uint i = 0; i < assets.size(); i++) {
		if (!Resource::loadTexture(("assets/"+assets[i]).c_str(), *textures[i])) {
			cerr << "Failed to load " << assets[i] << endl;
			glfwTerminate();
			return false;
		}
	}
	cerr << "Resources loaded in " << (glfwGetTime() - time) * 1000 << "ms" << endl;

    //Load BasicShader
	basicShader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");
	Shader::save("Base", basicShader);

	this->setState(GameState::MAIN_MENU);

    return true;
}

Game* Game::getInstance() 
{
    if(m_instance == nullptr)
    {
		cerr << "New instance" << endl;
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
	for (auto image : images) { delete image; } images.clear();
	for (auto button : buttons) { delete button; } buttons.clear();
	for (auto label : labels) { delete label; } labels.clear();
	char* bomberFont = (char*)"assets/fonts/bomberman.ttf";
	switch (state)
	{
	/**
	 * @brief Load the main menu
	 */
	case GameState::MAIN_MENU: {
		cerr << "Loading main menu..." << endl; float time = glfwGetTime();
		if (m_gameState == GameState::GAME) {
			// Delete game content
			delete map;
			map = nullptr;
		}
		/* Load Buttons */
		// Create button(window, position, anchor, size, ...)
		buttons.push_back(new Button(mainWindow, vec2(0, 50), vec2(0.5f, 0.5f), vec2(475, 75), (char *)"assets/button.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[0]->setLabel(Label(mainWindow, vec2(0, 50), vec2(0.5f, 0.5f), "Singleplayer", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[0]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->setState(GameState::SINGLEPLAYER);
		});

		buttons.push_back(new Button(mainWindow, vec2(0, -50), vec2(0.5f, 0.5f), vec2(475, 75), (char *)"assets/button.png", vec3(0.25f), vec3(0.25f, 0.25f, 0.25f), vec3(0.25f)));
		buttons[1]->setLabel(Label(mainWindow, vec2(0, -50), vec2(0.5f, 0.5f), "Multiplayer", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[1]->setOnClickCallback([]() {
			cerr << "Multiplayer clicked" << endl;
		});

		buttons.push_back(new Button(mainWindow, vec2(-50, 50), vec2(1.0f, 0.0f), vec2(79, 79), (char *)"assets/options.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[2]->setNineSlice(0);
		buttons[2]->setOnClickCallback([]() {
			cerr << "Options clicked" << endl;
		});

		/* Load Images */
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(WINDOW_W), &Textures::homeBackground));

		cerr << "Loaded main menu in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;

	/**
	 * @brief Load the singeplayer menu
	 */
	case GameState::SINGLEPLAYER: {
		cerr << "Loading singleplayer menu..." << endl; float time = glfwGetTime();
		/* Load Buttons */
		buttons.push_back(new Button(mainWindow, vec2(-122.5f, 50.0f), vec2(1.0f, 0.0f), vec2(220, 75), (char *)"assets/bluetton.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[0]->setLabel(Label(mainWindow, vec2(-122.5f, 50.0f), vec2(1.0f, 0.0f), "Launch Game", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[0]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->setState(GameState::GAME);
		});

		buttons.push_back(new Button(mainWindow, vec2(122.5f, 50.0f), vec2(0.0f, 0.0f), vec2(220, 75), (char *)"assets/bluetton.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[1]->setLabel(Label(mainWindow, vec2(122.5f, 50.0f), vec2(0.0f, 0.0f), "Go back", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[1]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->setState(GameState::MAIN_MENU);
		});

		char* arrow = (char*)"assets/arrow.png";
		char* reverse_arrow = (char*)"assets/reverse_arrow.png";
		buttons.push_back(new Button(mainWindow, vec2(-40, 120)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, 60)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, 0)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, -60)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, -120), vec2(0.75f, 0.5f), vec2(60), arrow, vec3(0.5f), vec3(0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-160, 120), vec2(0.75f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-160, 60)	, vec2(0.75f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-160, 0)	, vec2(0.75f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-160, -60), vec2(0.75f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-160, -120), vec2(0.75f, 0.5f), vec2(60), reverse_arrow, vec3(0.5f), vec3(0.5f), vec3(0.5f)));
		for (int i=2; i<12; i++) {
			buttons[i]->setNineSlice(0);
			buttons[i]->setOnClickCallback([]() {
				cerr << "Arrow clicked" << endl;
			});
		}

		/* Load Labels */
		labels.push_back(new Label(mainWindow, vec2(20, 120)	, vec2(0.25f, 0.5f), "Taille de la map"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 60)		, vec2(0.25f, 0.5f), "Nombre de joueurs"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 0)		, vec2(0.25f, 0.5f), "Pourcentage de murs"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, -60)	, vec2(0.25f, 0.5f), "Pourcentage de bonus"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, -120)	, vec2(0.25f, 0.5f), "Nombre de bots"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));

		labels.push_back(new Label(mainWindow, vec2(-100, 120)	, vec2(0.75f, 0.5f), "13"	, 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, 60)	, vec2(0.75f, 0.5f), "4"	, 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, 0)	, vec2(0.75f, 0.5f), "90"	, 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, -60)	, vec2(0.75f, 0.5f), "10"	, 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, -120)	, vec2(0.75f, 0.5f), "3"	, 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE, vec3(0.5f, 0.5f, 0.5f)));

		/* Load Images */
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(WINDOW_W/2), &Textures::blueRectangle));
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.0f, 0.5f), vec2(WINDOW_W/4), &Textures::bomberboy1));
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(WINDOW_W), &Textures::spaceBackground));

		cerr << "Loaded singleplayer menu in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;
	
	/**
	 * @brief Launch the game
	 */
	case GameState::GAME: {
		cerr << "Loading game..." << endl; float time = glfwGetTime();
		/* Load Buttons */
		buttons.push_back(new Button(mainWindow, vec2(162.5f, 50.0f), vec2(0.0f, 0.0f), vec2(300, 75), (char *)"assets/button.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[0]->setLabel(Label(mainWindow, vec2(162.5f, 50.0f), vec2(0.0f, 0.0f), "Exit to main menu", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[0]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->setState(GameState::MAIN_MENU);
		});

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
		
		mainCamera->getTransform().setPosition(vec3(-6.0f, -12.0f, -16.0f));
		mainCamera->getTransform().setRotation(vec3(0.90f, 0.0f, 0.0f));

		cerr << "Loaded game in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;
	}

	m_gameState = state;
}

void Game::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_deltaTime = glfwGetTime() - m_currentTime;
	m_currentTime = glfwGetTime();

	processInputs(mainWindow->getWindow());

	Wall* wall = new Wall(map); wall->draw(); delete wall;
	for (auto label : labels) { label->draw(); }
	for (auto button : buttons) { button->draw(); }
	for (auto image : images) { image->draw(); }
	switch (m_gameState)
	{
		case GameState::MAIN_MENU: {
		} break;

		case GameState::SINGLEPLAYER: {
		} break;

		case GameState::GAME: {
			map->update(m_deltaTime);
			map->draw();
		} break;
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		const float rotationSpeed = 0.003f;
		cameraTransform->rotate(vec3(0.0f, 1.0f, 0.0f) * cameraTransform->getRotation() * rotationSpeed * deltaX);
		cameraTransform->rotate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->getRotation() * rotationSpeed * deltaY);
	}
	// Display camera transform
	//cerr << "Camera position : " << cameraTransform->getPosition().x << " " << cameraTransform->getPosition().y << " " << cameraTransform->getPosition().z << endl;
	//cerr << "Camera rotation : " << cameraTransform->getRotation().x << " " << cameraTransform->getRotation().y << " " << cameraTransform->getRotation().z << endl;

	m_mousePos = vec2(xpos, ypos);
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}