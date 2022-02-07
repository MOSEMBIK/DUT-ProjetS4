#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Renderer/Geometry/Quad.h>
#include <linmath.h>

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
 
        glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quad.Draw();

		glfwSwapBuffers(window);

		// Récupérer les events tels que les clicks, les touches claviers etc...
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
