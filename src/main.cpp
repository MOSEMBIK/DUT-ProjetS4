#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Renderer/Shader.h>
#include <Renderer/Geometry/Primitives.h>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "An Error has occured: %s\n", description);
}

int main(int argc, char **argv)
{
	GLFWwindow* window;

	// Initialise la librairie GLFW
	if (!glfwInit())
		return -1;
		
	// Définit la fonction appelée lors d'erreur
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
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

	Mesh cube = Primitives::Cube();
	
	// VSync (0 = No VSync (Pas de limite), 1 = VSync (Basé sur la vitesse de l'écran (60Hz => 60fps)), 2 = Double VSync (Moitié de la vitesse de l'écran) )
	glfwSwapInterval(1);

	float time = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	int fps = 0;

	Shader basicShader("shader/vertex.glsl", "shader/fragment.glsl");

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
		glm::mat4 P = glm::perspective(glm::pi<float>() / 4.0f, (float)width / height, 0.1f, 100.0f);
		
		// Création de la matrice de vue (Caméra)
		glm::mat4 V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.5f));
		
		// Création de la matrice du modèle (Objet)
		glm::mat4 M = glm::rotate(glm::mat4(1.0f), time * 2, glm::vec3(1.0f, 1.0f, 1.0f));

		basicShader.use();
		basicShader.SetUniformValue("_ambientColor", vec3(1.0f, 1.0f, 1.0f));
		basicShader.SetUniformValue("_ambientStrength", 0.2f);
		basicShader.SetUniformValue("_lightPos", vec3(50.0f, 20.0f, 10.0f));
		basicShader.SetUniformValue("_M", M);
		basicShader.SetUniformValue("_V", V);
		basicShader.SetUniformValue("_P", P);
		cube.Draw(basicShader);

		glfwSwapBuffers(window);

		// Récupérer les events tels que les clicks, les touches claviers etc...
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
