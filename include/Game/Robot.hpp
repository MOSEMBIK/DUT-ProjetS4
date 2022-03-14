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
	~Robot() {};

	void update(float deltaTime);
	std::vector<glm::ivec2> move(glm::ivec2);
};
