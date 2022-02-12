#define STB_IMAGE_IMPLEMENTATION
#define UNUSED(x) (void)(x)

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Renderer/Shader.h>
#include <Renderer/Primitives.h>
#include <Renderer/Transform.h>

#include <glm/mat4x4.hpp>
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

	Mesh cube = Primitives::Cube("assets/diffuse.png", "assets/specular.png");
	
	// VSync (0 = No VSync (Pas de limite), 1 = VSync (Basé sur la vitesse de l'écran (60Hz => 60fps)), 2 = Double VSync (Moitié de la vitesse de l'écran) )
	glfwSwapInterval(1);

	float time = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	int fps = 0;

	Shader basicShader("shader/vertex.glsl", "shader/fragment.glsl");
	Transform transform;

	// Tant que la fenêtre ne doit pas être fermer (Alt-F4 ou click sur la croix par exemple)
	while (!glfwWindowShouldClose(window))
	{
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

		// Le delta time définit le temps qu'il s'est passé depuis la dernière update
		float deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		
		int avgFPS = fps;
		fps = 1 / deltaTime;
		avgFPS = (avgFPS + fps) / 2;

		std::string fpsCount = "FPS :";
		fpsCount += std::to_string(avgFPS);

		glfwSetWindowTitle(window, fpsCount.c_str());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Création de la matrice de projection
		mat4 P = perspective(pi<float>() / 4.0f, (float)width / height, 0.1f, 100.0f);
		
		// Création de la matrice de vue (Caméra)
		vec3 cameraPosition = vec3(0.0f, 0.0f, -7.5f);
		mat4 V = translate(mat4(1.0f), cameraPosition);

		transform.Rotate(vec3(1.0f, 1.0f, 1.0f) * deltaTime * pi<float>() / 4.0f);
		transform.SetScale(vec3(1.0f) + vec3(1.0f) * (sinf(time) + 1) * 0.25f);

		mat4 M = transform.GetTRSMatrix();
		
		// Création de la matrice du modèle (Objet)
		
		basicShader.Use();
		basicShader.SetUniformValue("_M", M);
		basicShader.SetUniformValue("_iTM", mat3(transpose(inverse(M))));
		basicShader.SetUniformValue("_V", V);
		basicShader.SetUniformValue("_P", P);

		basicShader.SetUniformValue("_light.diffuse", vec3(0.5f, 0.5f, 0.5f));
		basicShader.SetUniformValue("_light.specular", vec3(1.0f, 1.0f, 1.0f));
		basicShader.SetUniformValue("_light.position", vec3(0.0f, 1.0f, 5.0f));

		basicShader.SetUniformValue("_cameraPos", -cameraPosition);

		basicShader.SetUniformValue("_material.ambient", vec3(1.0f, 0.5f, 0.31f));
		basicShader.SetUniformValue("_material.diffuse_color", vec3(1.0f, 1.0f, 1.0f));
		basicShader.SetUniformValue("_material.specular_color", vec3(1.0f, 1.0f, 1.0f));
		basicShader.SetUniformValue("_material.shininess", 32.0f);
		cube.Draw(basicShader);

		glfwSwapBuffers(window);

		// Récupérer les events tels que les clicks, les touches claviers etc...
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
