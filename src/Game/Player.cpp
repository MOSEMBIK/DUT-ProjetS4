#include <Game/Player.hpp>
#include <Game/Map.hpp>

Player::Player(Map* map) : Actor(map,"assets/models/Bomber.obj") {
	this->m_materials[0].setDiffuseColor(glm::vec3(rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f));
}

void Player::setBomb(glm::ivec2 coord){
    map->addBomb( new Bomb(map, glm::vec3(0.0f,0.0f,0.5f)),    coord );
}