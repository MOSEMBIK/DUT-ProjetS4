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
	std::vector<glm::ivec2> genTrajet(glm::ivec2);
};
