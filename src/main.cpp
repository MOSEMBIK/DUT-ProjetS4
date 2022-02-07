#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Quad.h"
#include "linmath.h"

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

	// Tant que la fenêtre ne doit pas être fermer (Alt-F4 ou click sur la croix par exemple)
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		quad.Draw();

		glfwSwapBuffers(window);

		// Récupérer les events tels que les clicks, les touches claviers etc...
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
