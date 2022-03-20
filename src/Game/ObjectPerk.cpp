#include <Game/ObjectPerk.hpp>
#include <Game/Map.hpp>
#include <Engine/Primitives.hpp>


ObjectPerk::ObjectPerk(Map* map, glm::ivec2 coord, Type type) : Actor(map) {
	m_meshes.push_back(Primitives::quad());
	Material mat;
	switch (type) {
	case Type::Count:
		Resource::loadTexture("assets/bonus/count_up.png", Textures::countUp);
		mat.setDiffuseTexture(Textures::countUp->m_id);
		break;
	case Type::Range:
		Resource::loadTexture("assets/bonus/range_up.png", Textures::rangeUp);
		mat.setDiffuseTexture(Textures::rangeUp->m_id);
		break;
	case Type::Speed:
		Resource::loadTexture("assets/bonus/speed_up.png", Textures::speedUp);
		mat.setDiffuseTexture(Textures::speedUp->m_id);
		break;
	default:
		break;
	}
	m_materials.push_back(mat);

    this->lifetime = 50;
    this->type = type;
    m_transform.setPosition(glm::vec3(coord.x,0,coord.y));
    map->addBonus(this, coord);
}
