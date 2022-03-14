#pragma once

#include <Game/Actor.hpp>
#include <Game/Bomb.hpp>

#include <glm/vec3.hpp>

class Player: public Actor
{
private:
	std::vector<Bomb> launchedBombs;
	glm::vec3 color;
	float speed;
	int bombRange;
	int bombCount;

public:
	Player(Map* map);
	virtual ~Player() {};

	inline float getSpeed() const { return speed; }
	
};
