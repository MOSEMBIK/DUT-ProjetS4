#include <Game/Player.hpp>
#include <Game/Map.hpp>

using namespace std;
using namespace glm;

Player::Player(Map* map) : Actor(map,"assets/models/Bomber.obj") {
	this->m_materials[0].setDiffuseColor(glm::vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f));
}

void Player::setBomb(glm::ivec2 coord){
    map->addBomb( new Bomb(map, glm::vec3(0.0f,0.0f,0.5f)),    coord );
	cerr << "Fire is in the hole !" << endl;
}

void Player::update(float deltaTime) {
	glm::vec3 pos = m_transform.getPosition();
	if (pos.x > x) pos.x += 0.9f;
	if (pos.z > z) pos.z += 0.9f;

	if (int(pos.x) == x && int(pos.z) == z) {
		if (movement != '0') {
			std::cerr << "destPos: " << (int)(pos.x) << " " << (int)(pos.z) << std::endl;
			std::cerr << "my destPos: " << x << " " << z << std::endl;
			//m_transform.setPosition(glm::vec3(x, 0.0f, z));
		}
		movement = '0';
	}

	glm::quat targetRotation = m_transform.getRotation();
	if (int(pos.x) < x) {
		m_transform.translate(glm::vec3(1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	}
	else if (int(pos.x) > x) {
		m_transform.translate(glm::vec3(-1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, 0, 0));
	}
	else if (int(pos.z) < z) {
		m_transform.translate(glm::vec3(0, 0, 1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(90.0f), 0));
	}
	else if (int(pos.z) > z) {
		m_transform.translate(glm::vec3(0, 0, -1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(-90.0f), 0));
	}
	m_transform.setRotation(glm::slerp(m_transform.getRotation(), targetRotation, 6.0f / 60.0f));
}

Player::Player(Map* map) : Actor(map,"assets/models/Bomber.obj"),
	color(glm::vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f))
{
	this->m_materials[0].setDiffuseColor(color);
}

string Player::getData() {
	ivec3 pos = getTransform().getPosition();

	return string(
		"[" +
		to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.z) + "," +
		to_string(color.x) + "," + to_string(color.y) + "," + to_string(color.z) + "," +
		to_string(speed) + "," + to_string(bombRange) +
		"]"
	);
}
