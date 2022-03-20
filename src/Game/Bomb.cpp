
#include <Game/Bomb.hpp>
#include <Game/Map.hpp>

Bomb::Bomb(Map* map, glm::vec3 color, int range) : Actor(map,"assets/models/Bomb.obj"), color(color), range(range) {
	this->m_materials[0].setDiffuseColor(color);
}


void Bomb::onExplode() {
	glm::vec3 pos = this->getTransform().getPosition();
	map->onExplosion(pos.x, pos.z, range, duration);
	map->removeBomb(glm::ivec2(pos.x, pos.z));
	delete this;
}

void Bomb::update(float deltaTime) {
	timer -= deltaTime;
	if (timer <= 0) {
		onExplode();
	}
}
