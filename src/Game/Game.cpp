#include <Game/Game.hpp>
#include <Game/Robot.hpp>

#include <Engine/UI/Button.hpp>
#include <Engine/UI/Image.hpp>
#include <Engine/UI/Label.hpp>
#include <Engine/Transform.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>
#include <Engine/Utils.hpp>

#include <iostream>

using namespace glm;
using namespace std;
using std::cerr;
using std::endl;

#define SHOW_EVENTS 0

vector<Button*> buttons;
vector<Label*> labels;
vector<Image*> images;

Wall* wall;

Game* Game::m_instance = nullptr;

Game::Game() : m_currentTime(0), m_deltaTime(0), m_directionalLight(vec3(-1.0f, -1.0f, -1.0f), vec3(0.1f), vec3(1.0f), vec3(1.0f)), m_gameState(GameState::MAIN_MENU)
{
    m_currentTime = glfwGetTime();
	
    init();
}

void Game::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Game::onClose, this, std::placeholders::_1));
	dispatcher.Dispatch<AppUpdateEvent>(std::bind(&Game::onUpdate, this, std::placeholders::_1));

	mainWindow->onEvent(e);

	#if SHOW_EVENTS >= 1
		#if SHOW_EVENTS < 2
		if(e.GetEventType() != EventType::AppUpdate)
		#endif
			std::cout << e.ToString() << std::endl;
	#endif
}

bool Game::onClose(WindowCloseEvent& e)
{
	UNUSED(e);
	m_running = false;
	return true;
}

void Game::run()
{
	while(m_running)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainWindow->onUpdate();
	}
}

bool Game::init()
{
	mainWindow = Window::create();
	mainWindow->setEventCallback(std::bind(&Game::onEvent, this, std::placeholders::_1));

	mainCamera = new Camera();
	mainWindow->attachEventReceiver(*mainCamera);

	loadRequiredResources();

	// Make the lights
	m_directionalLight = DirectionalLight(vec3(+1.0f, -0.75f, -1.5f), vec3(0.1f), vec3(1.0f), vec3(1.0f));

	for (int i = 0; i < 4; i++)
		m_pointsLights.push_back(PointLight(i));
	m_directionalLight.sendToShader(*basicShader);
	for (PointLight pointLight : m_pointsLights)
		pointLight.sendToShader(*basicShader);

	m_lastTime = m_currentTime - 1;

	cerr << "Game initialized" << endl;
	return true;
}

bool Game::updateWindowOptions() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (m_fullscreen) {
		glfwSetWindowAttrib(mainWindow->getWindow(), GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowSize(mainWindow->getWindow(), mode->width, mode->height);
		glfwSetWindowPos(mainWindow->getWindow(), 0, 0);
	}
	else {
		glfwSetWindowAttrib(mainWindow->getWindow(), GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowSize(mainWindow->getWindow(), m_windowSize.x, m_windowSize.y);
		if (mode->width != m_windowSize.x)
			glfwSetWindowPos(mainWindow->getWindow(), (mode->width-m_windowSize.x)/2, (mode->height-m_windowSize.y)/2);
	}

	ofstream file("options.txt", ofstream::out | ofstream::trunc);
	if (file.is_open()) {
		file << "username:" << m_username << endl
		<< "windowWidth:" << m_windowSize.x << endl
		<< "fullscreen:" << (m_fullscreen ? "true" : "false") << endl
		<< "enableVSync:" << (mainWindow->isVSync() ? "true" : "false") << endl;
		file.close();
	}
	return true;
}

bool Game::loadRequiredResources()
{
	cerr << "Loading resources..." << endl;
	float time = glfwGetTime();
	std::vector<string> assets = {
		"white_texture.png", "black_texture.png", "home_background.png",
		"space_background.png", "bomberboy_1.png", "bomberboy_2.png",
		"bomberboy_3.png", "blue_rectangle.png"
	};
	std::vector<Texture**> textures = {
		&Textures::whiteTexture, &Textures::blackTexture, &Textures::homeBackground,
		&Textures::spaceBackground, &Textures::bomberboy1, &Textures::bomberboy2,
		&Textures::bomberboy3, &Textures::blueRectangle
	};
	for (uint i = 0; i < assets.size(); i++) {
		if (!Resource::loadTexture(("assets/"+assets[i]).c_str(), (*textures[i]))) {
			cerr << "Failed to load " << assets[i] << endl;
			glfwTerminate();
			return false;
		}
	}
	cerr << "Resources loaded in " << (glfwGetTime() - time) * 1000 << "ms" << endl;

    //Load BasicShader
	basicShader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");
	Shader::save("Base", basicShader);

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
	cerr << endl;
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
			Game* game = Game::getInstance();
			game->setState(GameState::OPTIONS);
		});

		/* Load Images */
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(DEFAULT_WINDOW_W), Textures::homeBackground));

		cerr << "Loaded main menu in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;

	/**
	 * @brief Load the option menu
	 */
	case GameState::OPTIONS: {
		cerr << "Loading options menu..." << endl; float time = glfwGetTime();
		/* Load Buttons */
		buttons.push_back(new Button(mainWindow, vec2(-137.5f, 50.0f), vec2(1.0f, 0.0f), vec2(250, 75), (char *)"assets/bluetton.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[0]->setLabel(Label(mainWindow, vec2(-137.5f, 50.0f), vec2(1.0f, 0.0f), "Apply changes", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[0]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->updateWindowOptions();
		});

		buttons.push_back(new Button(mainWindow, vec2(137.5f, 50.0f), vec2(0.0f, 0.0f), vec2(250, 75), (char *)"assets/bluetton.png", vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons[1]->setLabel(Label(mainWindow, vec2(137.5f, 50.0f), vec2(0.0f, 0.0f), "Go back", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		buttons[1]->setOnClickCallback([]() {
			Game* game = Game::getInstance();
			game->setState(GameState::MAIN_MENU);
		});

		char* arrow = (char*)"assets/arrow.png";
		char* reverse_arrow = (char*)"assets/reverse_arrow.png";
		buttons.push_back(new Button(mainWindow, vec2(-40, 120)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, 60)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(-40, 0)	, vec2(0.75f, 0.5f), vec2(60), arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(0, 120)	, vec2(0.50f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(0, 60)	, vec2(0.50f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		buttons.push_back(new Button(mainWindow, vec2(0, 0)		, vec2(0.50f, 0.5f), vec2(60), reverse_arrow, vec3(1.0f), vec3(0.75f, 0.75f, 0.5f), vec3(0.5f)));
		for (int i=2; i<8; i++)
			buttons[i]->setNineSlice(0);
		buttons[2]->setOnClickCallback([this]() { m_fullscreen = !m_fullscreen; labels[3]->setText(m_fullscreen ? "ON" : "OFF"); });
		buttons[5]->setOnClickCallback([this]() { m_fullscreen = !m_fullscreen; labels[3]->setText(m_fullscreen ? "ON" : "OFF"); });
		buttons[3]->setOnClickCallback([this]() { m_windowSize += ivec2(320,180); labels[4]->setText(to_string(m_windowSize.x) + "x" + to_string(m_windowSize.y)); });
		buttons[6]->setOnClickCallback([this]() { m_windowSize -= ivec2(320,180); labels[4]->setText(to_string(m_windowSize.x) + "x" + to_string(m_windowSize.y)); });
		buttons[4]->setOnClickCallback([this]() { mainWindow->setVSync(!mainWindow->isVSync()); labels[5]->setText(mainWindow->isVSync() ? "ON" : "OFF"); });
		buttons[7]->setOnClickCallback([this]() { mainWindow->setVSync(!mainWindow->isVSync()); labels[5]->setText(mainWindow->isVSync() ? "ON" : "OFF"); });

		/* Load Labels */
		labels.push_back(new Label(mainWindow, vec2(20, 120)	, vec2(0.25f, 0.5f), "Fullscreen"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 60)		, vec2(0.25f, 0.5f), "Window size"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 0)		, vec2(0.25f, 0.5f), "VSync"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));

		labels.push_back(new Label(mainWindow, vec2(-20, 120)	, vec2(0.625f, 0.5f), m_fullscreen ? "ON" : "OFF", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-20, 60)	, vec2(0.625f, 0.5f), to_string(m_windowSize.x) + "x" + to_string(m_windowSize.y), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-20, 0)		, vec2(0.625f, 0.5f), mainWindow->isVSync() ? "ON" : "OFF", 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));

		/* Load Images */
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(DEFAULT_WINDOW_W/2), Textures::blueRectangle));
		images.push_back(new Image(mainWindow, vec2(40, 100), vec2(1.0f, 0.0f), vec2(DEFAULT_WINDOW_W/4), Textures::bomberboy2));
		images.push_back(new Image(mainWindow, vec2(0, 100), vec2(0.0f, 0.5f), vec2(DEFAULT_WINDOW_W/3), Textures::bomberboy3));
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(DEFAULT_WINDOW_W), Textures::spaceBackground));

		cerr << "Loaded singleplayer menu in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;

	/**
	 * @brief Load the singleplayer menu
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
		for (int i=2; i<12; i++)
			buttons[i]->setNineSlice(0);
		buttons[2]->setOnClickCallback([this]() { m_gameSettings[0]++; labels[5]->setText(to_string(m_gameSettings[0])); });
		buttons[7]->setOnClickCallback([this]() { m_gameSettings[0]--; labels[5]->setText(to_string(m_gameSettings[0])); });
		buttons[3]->setOnClickCallback([this]() { m_gameSettings[4] = m_gameSettings[1]++; labels[6]->setText(to_string(m_gameSettings[1])); labels[9]->setText(to_string(m_gameSettings[4])); });
		buttons[8]->setOnClickCallback([this]() { m_gameSettings[4] = --m_gameSettings[1] - 1; labels[6]->setText(to_string(m_gameSettings[1])); labels[9]->setText(to_string(m_gameSettings[4])); });
		buttons[4]->setOnClickCallback([this]() { m_gameSettings[2]++; if (m_gameSettings[2] == 101) m_gameSettings[2] = 0; labels[7]->setText(to_string(m_gameSettings[2])); });
		buttons[9]->setOnClickCallback([this]() { m_gameSettings[2]--; if (m_gameSettings[2] == 255) m_gameSettings[2] = 100; labels[7]->setText(to_string(m_gameSettings[2])); });
		buttons[5]->setOnClickCallback([this]() { m_gameSettings[3]++; if (m_gameSettings[3] == 101) m_gameSettings[3] = 0; labels[8]->setText(to_string(m_gameSettings[3])); });
		buttons[10]->setOnClickCallback([this]() { m_gameSettings[3]--; if (m_gameSettings[3] == 255) m_gameSettings[3] = 100; labels[8]->setText(to_string(m_gameSettings[3])); });

		/* Load Labels */
		labels.push_back(new Label(mainWindow, vec2(20, 120)	, vec2(0.25f, 0.5f), "Size of the map"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 60)		, vec2(0.25f, 0.5f), "Number of players"	, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, 0)		, vec2(0.25f, 0.5f), "Wall percentage"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, -60)	, vec2(0.25f, 0.5f), "Bonus percentage"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(20, -120)	, vec2(0.25f, 0.5f), "Number of bots"		, 24, bomberFont, ALIGN_LEFT | ALIGN_MIDDLE));

		m_gameSettings[4] = m_gameSettings[1] - 1;
		labels.push_back(new Label(mainWindow, vec2(-100, 120)	, vec2(0.75f, 0.5f), to_string(m_gameSettings[0]).c_str(), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, 60)	, vec2(0.75f, 0.5f), to_string(m_gameSettings[1]).c_str(), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, 0)	, vec2(0.75f, 0.5f), to_string(m_gameSettings[2]).c_str(), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, -60)	, vec2(0.75f, 0.5f), to_string(m_gameSettings[3]).c_str(), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE));
		labels.push_back(new Label(mainWindow, vec2(-100, -120)	, vec2(0.75f, 0.5f), to_string(m_gameSettings[4]).c_str(), 24, bomberFont, ALIGN_CENTER | ALIGN_MIDDLE, vec3(0.5f, 0.5f, 0.5f)));

		/* Load Images */
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(DEFAULT_WINDOW_W/2), Textures::blueRectangle));
		images.push_back(new Image(mainWindow, vec2(40, 100), vec2(1.0f, 0.0f), vec2(DEFAULT_WINDOW_W/4), Textures::bomberboy2));
		images.push_back(new Image(mainWindow, vec2(0, 100), vec2(0.0f, 0.5f), vec2(DEFAULT_WINDOW_W/3), Textures::bomberboy3));
		images.push_back(new Image(mainWindow, vec2(0, 0), vec2(0.5f, 0.5f), vec2(DEFAULT_WINDOW_W), Textures::spaceBackground));

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
		map->generateMap(m_gameSettings[0], m_gameSettings[2]);
		
		// Test de robots
		for (int i=0; i < m_gameSettings[4]; i++) {
			Robot* robot = new Robot(map);
			robot->getTransform().setPosition(vec3(1.0f, 0.0f, 1.0f) * float(rand()%m_gameSettings[0]));
			map->addActor(robot);
		}

		mainCamera->getTransform().setPosition(vec3(-6.0f, -12.0f, -16.0f));
		mainCamera->getTransform().setRotation(vec3(0.90f, 0.0f, 0.0f));

		cerr << "Loaded game in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
	} break;
	}

	m_gameState = state;
}

bool Game::postInit() {
	//Load options
	fstream file("options.txt");
	if (!file.is_open()) {
		ofstream outfile("options.txt");
		outfile << "username:PielleBoule" << endl << "windowWidth:1280" << endl
			<< "fullscreen:false" << endl << "enableVSync:true" << endl;
		outfile.close();
	}

	cerr << "Loading options..." << endl;
	string line;
	string value;
	std::size_t i;
	ifstream optionsFile("options.txt");
	if (optionsFile.is_open()) {
		getline(optionsFile, line); i = line.find(':'); value = line.substr(i + 1, line.length() - i + 1); m_username = value;
		getline(optionsFile, line); i = line.find(':'); value = line.substr(i + 1, line.length() - i + 1); m_windowSize = vec2(stoi(value), stoi(value)/16*9);
		getline(optionsFile, line); i = line.find(':'); value = line.substr(i + 1, line.length() - i + 1); m_fullscreen = (value == "true");
		getline(optionsFile, line); i = line.find(':'); value = line.substr(i + 1, line.length() - i + 1); mainWindow->setVSync(value == "true");
		optionsFile.close();
	}
	updateWindowOptions();

	wall = new Wall(map);
	wall->getTransform().setScale(vec3(0.0f));
	this->setState(GameState::MAIN_MENU);
	return true;
}

bool Game::onUpdate(AppUpdateEvent& e)
{
	m_deltaTime = e.GetDeltaTime();
	m_currentTime = glfwGetTime();

	wall->draw();
	for (auto label : labels) { label->draw(); }
	for (auto button : buttons) { button->draw(); }
	for (auto image : images) { image->draw(); }
	switch (m_gameState)
	{
		case GameState::MAIN_MENU: {
		} break;

		case GameState::OPTIONS: {
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
		std::string fpsCount = "FPS: ";
		fpsCount += std::to_string((int)(1 / m_deltaTime));
		mainWindow->setTitle(fpsCount);
		m_lastTime = m_currentTime;
	}

	return false;
}
