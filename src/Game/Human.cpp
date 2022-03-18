#include <Game/Human.hpp>
#include <Game/Map.hpp>

Human::Human(Map* map) : Player(map) {
	this->mapSize = map->getSize() - 1;
	this->x = rand() % mapSize;
	this->z = rand() % mapSize;
}

void Human::update(float deltaTime) {
	Transform& transform = getTransform();

	glm::quat targetRotation = transform.getRotation();
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS){
        transform.translate(glm::vec3(1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
    }
	else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
		transform.translate(glm::vec3(-1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, 0, 0));
	}
	else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		transform.translate(glm::vec3(0, 0, 1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(90.0f), 0));
	}
	else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
		transform.translate(glm::vec3(0, 0, -1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(-90.0f), 0));
	}
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->setBomb(glm::ivec2(m_transform.getPosition().x, m_transform.getPosition().z));
    } 
	transform.setRotation(glm::slerp(transform.getRotation(), targetRotation, 6.0f / 60.0f));
}