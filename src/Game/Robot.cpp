#include <Game/Robot.hpp>

Robot::Robot() {}


void Robot::update(float deltaTime) {
	Transform& transform = getTransform();
	glm::vec3 pos = transform.GetPosition();
	if (int(pos.x) == x && int(pos.z) == z) {
		x = rand() % 12;
		z = rand() % 12;
	}

	if (int(pos.x) < x) {
		transform.Translate(glm::vec3(1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.x) > x) {
		transform.Translate(glm::vec3(-1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.z) < z) {
		transform.Translate(glm::vec3(0, 0, 1.0f) * deltaTime * (2 + getSpeed()));
	}
	else if (int(pos.z) > z) {
		transform.Translate(glm::vec3(0, 0, -1.0f) * deltaTime * (2 + getSpeed()));
	}
}


