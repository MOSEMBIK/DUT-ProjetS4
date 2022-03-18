
#include <Engine/Primitives.hpp>
#include <Engine/Material.hpp>
#include <Game/Wall.hpp>
#include <Game/Map.hpp>

using namespace std;
using namespace glm;


Wall::Wall(Map* map, Type type): Actor(map), type(type) {
	m_meshes.push_back(Primitives::cube());
	Material mat;
	switch (type) {
	case Type::Metal:
		this->health = -1;
		mat.setDiffuseColor(vec3(0.5f, 0.5f, 0.5f));
		mat.setSpecularColor(vec3(0.6f, 0.6f, 0.6f));
		break;
	case Type::Stone:
		mat.setDiffuseColor(vec3(0.2f, 0.2f, 0.2f));
		mat.setSpecularColor(vec3(0.3f, 0.3f, 0.3f));
		this->health = 3;
		break;
	case Type::Wood:
		mat.setDiffuseColor(vec3(0.86f, 0.72f, 0.52f));
		mat.setSpecularColor(vec3(0.9f, 0.76f, 0.58f));
		this->health = 1;
		break;
	}
	m_materials.push_back(mat);
}

Wall::Wall(Map* map, string& data) : Actor(map) {
	// data string example : [2,0,5,2,1]
	vector<string> wallData;
	for (int i = 0; i < (int)data.size(); i++) {
		if (data[i] == ',') {
			wallData.push_back(data.substr(0, i));
			data = data.substr(i + 1, data.size() - i - 1);
			i = 0;
		}
	} wallData.push_back(data);

	m_meshes.push_back(Primitives::cube());
	Material mat;
	this->m_transform.setPosition(vec3(stoi(wallData[0]), stoi(wallData[1]), stoi(wallData[2])));

	this->type = Type(stoi(wallData[3]));
	switch (type) {
	case Type::Metal:
		mat.setDiffuseColor(vec3(0.5f, 0.5f, 0.5f));
		mat.setSpecularColor(vec3(0.6f, 0.6f, 0.6f));
		break;
	case Type::Stone:
		mat.setDiffuseColor(vec3(0.2f, 0.2f, 0.2f));
		mat.setSpecularColor(vec3(0.3f, 0.3f, 0.3f));
		break;
	case Type::Wood:
		mat.setDiffuseColor(vec3(0.86f, 0.72f, 0.52f));
		mat.setSpecularColor(vec3(0.9f, 0.76f, 0.58f));
		break;
	}
	m_materials.push_back(mat);
	this->health = stoi(wallData[4]);
}

string Wall::getData() {
	ivec3 pos = getTransform().getPosition();
	string wallType;
	switch (this->type) {
	case Type::Metal: wallType = "0"; break;
	case Type::Stone: wallType = "1"; break;
	case Type::Wood: wallType = "2"; break;
	}

	return string(
		"[" +
		to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.z) + "," +
		wallType + "," + to_string(health) +
		"]"
	);
} 

void Wall::onDestroy() {
	// summon un bonus ?
	// Destroy le wall
	ivec2 pos(m_transform.getPosition().x, m_transform.getPosition().z);
	map->removeWall(pos);
	delete this;
}

void Wall::removeHealth() {
	if (this->health >= 1) {
		if (type == Wall::Type::Stone) {
			this->m_materials[0].setDiffuseColor(vec3(0.2f/health*3.0f));
		}
		this->health -= 1;
		if (this->health == 0)
			this->onDestroy();
	}
}
