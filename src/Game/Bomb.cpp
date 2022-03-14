
#include <Game/Bomb.hpp>
#include <Game/Map.hpp>

Bomb::Bomb(Map* map, glm::vec3 color, int range, int duration) : Actor(map,"assets/models/Bomb.obj"), color(color), range(range), duration(duration) {
	this->m_materials[0].setDiffuseColor(color);
}


void Bomb::onExplode() {
	glm::vec3 pos = this->getTransform().getPosition();
	this->map->onExplosion(pos.x, pos.z, range);
}