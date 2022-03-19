#include <Game/ObjectPerk.hpp>
#include <Game/Map.hpp>


ObjectPerk::ObjectPerk(Map* map, glm::ivec2 coord, Type type) : Actor(map,"assets/models/ResTorus.obj") {
    this->lifetime = 100;
    this->type = type;
    m_transform.setPosition(glm::vec3(coord.x,0,coord.y));
    map->addBonus(this, coord);
}
