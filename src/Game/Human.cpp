#include <Game/Human.hpp>
#include <Game/Map.hpp>

Human::Human(Map* map) : Player(map) {
	this->mapSize = map->getSize() - 1;
	this->x = m_transform.getPosition().x;
	this->z = m_transform.getPosition().z;
}

void Human::update(float deltaTime) {
	glm::vec3 pos = m_transform.getPosition();

	unsigned int oldKeyPressed = keyPressed;
	keyPressed = 0;
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		keyPressed = GLFW_KEY_RIGHT;
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		keyPressed = GLFW_KEY_LEFT;
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
		keyPressed = GLFW_KEY_DOWN;
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
		keyPressed = GLFW_KEY_UP;
	
	switch (oldKeyPressed) {
	case GLFW_KEY_RIGHT:
		if (movement != 'U' && movement != 'D' && map->isReachable(glm::ivec2(pos.x + 1, pos.z))) {
			movement = 'R';
        	x = pos.x + 1;
			z = pos.z;
    	}
		break;
	case GLFW_KEY_LEFT:
		if (movement != 'U' && movement != 'D' && map->isReachable(glm::ivec2(pos.x - 0.1f, pos.z))) {
			movement = 'L';
			x = pos.x - 0.1f;
			z = pos.z;
		}
		break;
	case GLFW_KEY_DOWN:
		if (movement != 'R' && movement != 'L' && map->isReachable(glm::ivec2(pos.x, pos.z + 1))) {
			movement = 'D';
			x = pos.x;
			z = pos.z + 1;
		}
		break;
	case GLFW_KEY_UP:
		if (movement != 'R' && movement != 'L' && map->isReachable(glm::ivec2(pos.x, pos.z - 0.1f))) {
			movement = 'U';
			x = pos.x;
			z = pos.z - 0.1f;
		}
		break;
	}

	if (spacePressed && glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE) {
		if (pos.x > x) pos.x += 0.9f;
		if (pos.z > z) pos.z += 0.9f;
		setBomb(glm::ivec2(pos.x, pos.z));
		spacePressed = false;
	}
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		spacePressed = true;

	Player::update(deltaTime);
}