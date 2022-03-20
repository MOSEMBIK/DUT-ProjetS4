
#include <Game/Bomb.hpp>
#include <Game/Map.hpp>

Bomb::Bomb(Map* map, glm::vec3 color, int range) : Actor(map,"assets/models/Bomb.obj"), color(color), range(range) {
	this->m_materials[0].setDiffuseColor(color);
}


void Bomb::onExplode() {
	glm::vec3 pos = this->getTransform().getPosition();
	map->onExplosion(pos.x, pos.z, range);
}

void Bomb::update(float deltaTime) {
	timer -= deltaTime;
	if (timer <= 0) {
		if (!exploded) {
			onExplode();
			exploded = true;
		}
		else {
			this->m_materials[0].setDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0f));
			duration -= deltaTime;
			if (duration <= 0) {
				glm::ivec2 pos(m_transform.getPosition().x, m_transform.getPosition().z);
				map->removeBomb(pos);
				delete this;
			}
		}
	}
}