#include <utils.h>

#include <map>

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
	std::cerr << "---------------------------" << std::endl;

	Game* game = Game::getInstance();
	game->postInit();
	game->run();

	glfwTerminate();

	return 0;
}