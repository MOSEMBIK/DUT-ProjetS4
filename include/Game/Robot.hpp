#pragma once

#include <Game/Player.hpp>

class Robot: public Player
{
private:
	int mapSize;
	int x, z;

	std::vector<glm::ivec2> trajet;
	int case_of_t;

public:
	Robot(Map* map);
	virtual ~Robot() {}

	void update(float deltaTime);

	glm::ivec2 choseDestination();

	std::vector<glm::ivec2> genTrajetMann(glm::ivec2);
	void setTrajet(std::vector<glm::ivec2>);
};
