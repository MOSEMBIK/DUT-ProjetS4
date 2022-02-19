#include <utils.h>

#include <Engine/Window.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/Transform.hpp>
#include <Engine/Lights.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>

#include <Game/Game.hpp>

#include <Game/Map.hpp>
#include <Game/Robot.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	// Initialise la librairie GLFW
	if (!glfwInit())
		return -1;

	Game* game = Game::getInstance();

    while(!glfwWindowShouldClose(game->getMainWindow()->GetWindow()))
    {
        game->update();
    }

	glfwTerminate();

	/*// Met le contexte de la fenêtre en contexte principal


	cout << glGetString(GL_VERSION) << endl;
	
	// VSync (0 = No VSync (Pas de limite), 1 = VSync (Basé sur la vitesse de l'écran (60Hz => 60fps)), 2 = Double VSync (Moitié de la vitesse de l'écran) )


	float time = glfwGetTime();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	Shader* basicShader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");
	Shader::Register("Base", *basicShader);
	Transform transform;

	Camera* camera =  Camera::GetInstance();

	Mesh cube = Primitives::Cube();

	vector<vec3> vertices, normals;
	vector<vec2> texCoords;
	vector<Material> materials;
	vector<Mesh> meshes;

	Resource::LoadOBJ("assets/models/ColoredCube.obj", meshes, materials);
	vector<Vertex> susanneVertex = Mesh::CreateFromVectors(vertices, normals, texCoords);
	Mesh susanne = Mesh(susanneVertex, GL_TRIANGLES);

	
	//pointLights[0].Enable();
	//pointLights[0].SetDiffuse(vec3(1.0f, 0.0f, 0.0f));
	//pointLights[0].SetSpecular(vec3(5.0f, 0.0f, 0.0f));
	//pointLights[0].SetPosition(vec3(0.0f, 0.0f, 10.0f));
	//pointLights[0].SetRange(50);
//
	//pointLights[1].Enable();
	//pointLights[1].SetDiffuse(vec3(0.0f, 0.0f, 2.0f));
	//pointLights[1].SetSpecular(vec3(0.0f, 0.0f, 10.0f));
	//pointLights[1].SetPosition(vec3(-10.0f, 0.0f, 10.0f));
	//pointLights[1].SetRange(50);
//
	//pointLights[2].Enable();
	//pointLights[2].SetDiffuse(vec3(0.0f, 0.5f, 0.0f));
	//pointLights[2].SetSpecular(vec3(0.0f, 10.0f, 0.0f));
	//pointLights[2].SetPosition(vec3(.0f, 10.0f, 10.0f));
	//pointLights[2].SetRange(50);
	

	glfwGetCursorPos(window, &oldMouseXPos, &oldMouseYPos);
	glfwSetKeyCallback(window, key_callback);

	float lastTime = time - 1;

	camera->GetTransform().SetPosition(vec3(-5.86219f, -13.4262f, -16.7321f));
	camera->GetTransform().SetRotation(vec3(0.446024f, 0.00986233f, -0.0049932f));

	// Tests bizarre avec la map
	Map map;
	map.generateMap(13);
	Actor resTorus;
	resTorus.loadOBJ("assets/models/ResTorus.obj");
	map.addActor(&resTorus);
	Robot* robot = new Robot(), *robot2 = new Robot();
	robot->loadOBJ("assets/models/Bomberman.obj");
	robot->getTransform().SetPosition(vec3(6.0f, 0.0f, 6.0f));
	map.addActor(robot);
	map.addActor(robot2);
	// Tant que la fenêtre ne doit pas être fermer (Alt-F4 ou click sur la croix par exemple)
	while (!glfwWindowShouldClose(window))
	{
		// Le delta time définit le temps qu'il s'est passé depuis la dernière update
		
		process_inputs(window, deltaTime);

		if(time - lastTime >= 1)
		{
			std::string fpsCount = "FPS :";
			fpsCount += std::to_string((int)(1 / deltaTime));
			glfwSetWindowTitle(window, fpsCount.c_str());
			lastTime = time;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Récupération de la matrice de projection
		mat4 P = camera->GetProjectionMatrix(window);

		// Création de la matrice de vue (Caméra)
		mat4 V = camera->GetViewMatrix();

		//transform.Rotate(vec3(1.0f, 1.0f, -1.0f) * deltaTime * pi<float>() / 4.0f);
		//transform.SetScale(vec3(1.0f) + vec3(1.0f) * (sinf(time) + 1) * 0.25f);

		mat4 M = transform.GetTRSMatrix();
		
		dirLight.SendToShader(*basicShader);
		for(PointLight pointLight : pointLights)
			pointLight.SendToShader(*basicShader);

		robot->update(deltaTime);
		robot2->update(deltaTime);
		resTorus.getTransform().Rotate(vec3(0.0f, 1.0f, 0.0f) * deltaTime * pi<float>());
		map.draw();
	}

	glfwTerminate();*/
	return 0;
}
