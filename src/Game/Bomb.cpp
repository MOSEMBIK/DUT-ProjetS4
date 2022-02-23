
#include <Game/Bomb.hpp>

Bomb::Bomb(Map* map, glm::vec3 color, int range, int duration) : Actor(map,"assets/models/Bomb.obj"), color(color), range(range), duration(duration) {
	this->m_materials[0].setDiffuseColor(color);
}


