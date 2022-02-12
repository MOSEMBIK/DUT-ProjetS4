#define STB_IMAGE_IMPLEMENTATION
#define UNUSED(x) (void)(x)

#include <Engine/Shader.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/Transform.hpp>
#include <Engine/Lights.hpp>
#include <Engine/Camera.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

void error_callback(int error, const char* description)
{
	UNUSED(error);

    fprintf(stderr, "An Error has occured: %s\n", description);
}

double oldMouseXPos;
double oldMouseYPos;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void process_inputs(GLFWwindow* window, float deltaTime)
{
	// Camera movement
	Transform* cameraTransform =  &Camera::GetInstance()->GetTransform();
    const float cameraSpeed = 5.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraTransform->Translate(vec3(0.0f, 0.0f, 1.0f) * cameraTransform->GetRotation() * cameraSpeed * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraTransform->Translate(vec3(0.0f, 0.0f, -1.0f) * cameraTransform->GetRotation() * cameraSpeed * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraTransform->Translate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * cameraSpeed * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraTransform->Translate(vec3(-1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * cameraSpeed * deltaTime);
		

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float deltaX = xpos - oldMouseXPos, deltaY = ypos - oldMouseYPos;

	// Camera rotation
	const float rotationSpeed = 2.0f;
	cameraTransform->Rotate(vec3(0.0f, 1.0f, 0.0f) * cameraTransform->GetRotation() * rotationSpeed * deltaX * deltaTime);
	cameraTransform->Rotate(vec3(1.0f, 0.0f, 0.0f) * cameraTransform->GetRotation() * rotationSpeed * deltaY * deltaTime);

	oldMouseXPos = xpos, oldMouseYPos = ypos;
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	GLFWwindow* window;

	// Initialise la librairie GLFW
	if (!glfwInit())
		return -1;
		
	// Définit la fonction appelée lors d'erreur
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	//glfwWindowHint(GLFW_DECORATED, 0);
	// Créer une fenêtre dans un contexte OpenGL
	window = glfwCreateWindow(800, 800, "Hello World !", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Met le contexte de la fenêtre en contexte principal
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		cout << "ERROR!" << endl;

	cout << glGetString(GL_VERSION) << endl;
	glClearColor(0, 0, 0, 0);

	Mesh cube = Primitives::Cube("assets/block_diffuse.png", "assets/block_specular.png");
	
	// VSync (0 = No VSync (Pas de limite), 1 = VSync (Basé sur la vitesse de l'écran (60Hz => 60fps)), 2 = Double VSync (Moitié de la vitesse de l'écran) )
	glfwSwapInterval(0);

	float time = glfwGetTime();

	glEnable(GL_CULL_FACE); 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	Camera* camera =  Camera::GetInstance();
	Shader basicShader("shader/vertex.glsl", "shader/fragment.glsl");
	Transform transform;

	DirectionalLight dirLight(vec3(-1.0f, -1.0f, -1.0f), vec3(0.1f), vec3(0.0f), vec3(0.0f));
	vector<PointLight> pointLights;
	for(int i = 0; i < 4; i++)
		pointLights.push_back(PointLight(i));
		
	pointLights[0].Enable();
	pointLights[0].SetDiffuse(vec3(1.0f, 0.0f, 0.0f));
	pointLights[0].SetSpecular(vec3(5.0f, 0.0f, 0.0f));
	pointLights[0].SetPosition(vec3(0.0f, 0.0f, 10.0f));
	pointLights[0].SetRange(50);

	pointLights[1].Enable();
	pointLights[1].SetDiffuse(vec3(0.0f, 0.0f, 2.0f));
	pointLights[1].SetSpecular(vec3(0.0f, 0.0f, 10.0f));
	pointLights[1].SetPosition(vec3(-10.0f, 0.0f, 10.0f));
	pointLights[1].SetRange(50);

	pointLights[2].Enable();
	pointLights[2].SetDiffuse(vec3(0.0f, 0.5f, 0.0f));
	pointLights[2].SetSpecular(vec3(0.0f, 10.0f, 0.0f));
	pointLights[2].SetPosition(vec3(.0f, 10.0f, 10.0f));
	pointLights[2].SetRange(50);

	basicShader.Use();
	dirLight.SendToShader(basicShader);
	for(int i = 0; i < 4; i++)
	{
		pointLights[i].SendToShader(basicShader);
	}

	glfwGetCursorPos(window, &oldMouseXPos, &oldMouseYPos);
	glfwSetKeyCallback(window, key_callback);

	float lastTime = time - 1;

	Camera::GetInstance()->GetTransform().Translate(vec3(0.0f, 0.0f, -7.5f));

	// Tant que la fenêtre ne doit pas être fermer (Alt-F4 ou click sur la croix par exemple)
	while (!glfwWindowShouldClose(window))
	{
		// Le delta time définit le temps qu'il s'est passé depuis la dernière update
		float deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		
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

		//transform.Rotate(vec3(1.0f, 1.0f, 1.0f) * deltaTime * pi<float>() / 4.0f);
		//transform.SetScale(vec3(1.0f) + vec3(1.0f) * (sinf(time) + 1) * 0.25f);

		mat4 M = transform.GetTRSMatrix();
		
		// Création de la matrice du modèle (Objet)
		basicShader.Use();
		basicShader.SetUniformValue("_M", M);
		basicShader.SetUniformValue("_iTM", mat3(transpose(inverse(M))));
		basicShader.SetUniformValue("_V", V);
		basicShader.SetUniformValue("_P", P);

		basicShader.SetUniformValue("_cameraPos", -camera->GetTransform().GetPosition());

		basicShader.SetUniformValue("_material.ambient", vec3(1.0f, 0.5f, 0.31f));
		basicShader.SetUniformValue("_material.color", vec3(1.0f, 1.0f, 1.0f));
		basicShader.SetUniformValue("_material.shininess", 32.0f);

		cube.Draw(basicShader);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
