#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Renderer/Program.h>
#include <Renderer/Geometry/Quad.h>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace Renderer;

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

	// Créer une fenêtre dans un contexte OpenGL
	window = glfwCreateWindow(720, 720, "Hello World !", NULL, NULL);
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

	Quad quad;
	Program program = Program(ProgramParams {
		{"position"},
		{ {"MVP", "mat4"} },
		{
			"precision mediump float;"
			"uniform float b;"
			"uniform mat4 MVP;"
			"attribute vec3 position;"
			"varying vec4 color;"
			"void main ()"
			"{"
				"gl_Position = MVP * vec4 (position, 1.0);"
				"color = vec4 (1.0, 1.0, 1.0, 1.0);"
			"}"
		},
		{
			"precision mediump float;"
			"varying vec4 color;"
			"void main ()"
			"{"
				"gl_FragColor = color;"
			"}"
		}
	});
	
	// VSync (0 = No VSync (Pas de limite), 1 = VSync (Basé sur la vitesse de l'écran (60Hz => 60fps)), 2 = Double VSync (Moitié de la vitesse de l'écran) )
	glfwSwapInterval(1);

	double time = glfwGetTime();

	// Tant que la fenêtre ne doit pas être fermer (Alt-F4 ou click sur la croix par exemple)
	while (!glfwWindowShouldClose(window))
	{
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

		// Le delta time définit le temps qu'il s'est passé depuis la dernière update
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		int fps = 1 / deltaTime;
		std::string fpsCount = "FPS :";
		fpsCount += std::to_string(fps);

		glfwSetWindowTitle(window, fpsCount.c_str());
 
        glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 P = glm::perspective(glm::pi<float>() / 4.0f, 1.0f, 0.1f, 100.0f);
		glm::mat4 V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::rotate(M, (float)glfwGetTime(),  glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 MVP = P * V * M;

		program.Use (
			{
				{"MVP", &MVP[0][0] }
			},
			[=](const Program* p) {
				quad.Draw();
			}
		);

		glfwSwapBuffers(window);

		// Récupérer les events tels que les clicks, les touches claviers etc...
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
