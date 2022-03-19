#include <Game/Player.hpp>
#include <Game/Map.hpp>

using namespace std;
using namespace glm;

Player::Player(Map* map) : Actor(map,"assets/models/Bomber.obj") {
	this->m_materials[0].setDiffuseColor(glm::vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f));
}

Player::Player(Map* map, string& data) : Actor(map,"assets/models/Bomber.obj") {
	vector<string> playerData;
	for (int i = 0; i < (int)data.size(); i++) {
		if (data[i] == ',') {
			playerData.push_back(data.substr(0, i));
			data = data.substr(i + 1, data.size() - i - 1);
			i = 0;
		}
	} playerData.push_back(data);

	m_transform.setPosition(vec3(stof(playerData[0]), stof(playerData[1]), stof(playerData[2])));
	m_transform.setEulerAngle(vec3(stof(playerData[3]), stof(playerData[4]), stof(playerData[5])));

	this->color = vec3(stof(playerData[6]), stof(playerData[7]), stof(playerData[8]));
	this->m_materials[0].setDiffuseColor(color);
	this->speed = stof(playerData[9]);
	this->bombRange = stoi(playerData[10]);
	this->id = stoi(playerData[11]);
}

void Player::setBomb(glm::ivec2 coord){
    map->addBomb( new Bomb(map, glm::vec3(0.0f,0.0f,0.5f)),    coord );
	cerr << "Fire is in the hole !" << endl;
}

void Player::update(float deltaTime) {
	glm::vec3 pos = m_transform.getPosition();
	ObjectPerk::Type type = map->pickUpBonus(glm::ivec2(pos.x,pos.z));
	if (type != ObjectPerk::Type::None){
		switch (type)
		{
		case ObjectPerk::Type::Count:
			this->bombCount++;
			break;
		case ObjectPerk::Type::Range:
			this->bombRange++;
			break;
		case ObjectPerk::Type::Speed:
			this->speed++;
			break;
		}
	}

	
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

string Player::getData() const {
	vec3 pos = m_transform.getPosition();
	vec3 rot = m_transform.getEuleurAngles();

	return string(
		"[" +
		to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.z) + "," +
		to_string(rot.x) + "," + to_string(rot.y) + "," + to_string(rot.z) + "," +
		to_string(color.x) + "," + to_string(color.y) + "," + to_string(color.z) + "," +
		to_string(speed) + "," + to_string(bombRange) + "," + to_string(id) +
		"]"
	);
}

string Player::getPosRot() const {
	vec3 pos = m_transform.getPosition();
	vec3 rot = m_transform.getEuleurAngles();

	return string(
		to_string(id) + "," +
		to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.z) + "," +
		to_string(rot.x) + "," + to_string(rot.y) + "," + to_string(rot.z)
	);
}

void Player::loadPosRot(vec3 pos, vec3 rot) {
	m_transform.setPosition(pos);
	m_transform.setRotation(rot);
}

void Player::upBombCount(){
	this->bombCount+=1;
}

void Player::upBombRange(){
	this->bombRange+=1;
}

