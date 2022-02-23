#include <Game/Robot.hpp>
#include <Game/Map.hpp>

Robot::Robot(Map* map) : Player(map) {
	this->mapSize = map->getSize() - 1;
	this->x = rand() % mapSize;
	this->z = rand() % mapSize;
}


void Robot::update(float deltaTime) {
	Transform& transform = getTransform();
	glm::vec3 pos = transform.getPosition();
	if (int(pos.x) == x && int(pos.z) == z) {
		x = rand() % mapSize;
		z = rand() % mapSize;
	}

	if (int(pos.x) < x) {
		transform.translate(glm::vec3(1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.x) > x) {
		transform.translate(glm::vec3(-1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.z) < z) {
		transform.translate(glm::vec3(0, 0, 1.0f) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.z) > z) {
		transform.translate(glm::vec3(0, 0, -1.0f) * deltaTime * (2 + getSpeed()));
	}
}


