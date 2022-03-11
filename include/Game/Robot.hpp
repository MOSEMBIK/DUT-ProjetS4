#pragma once

#include <Game/Player.hpp>

class Robot: public Player
{
private:
	int mapSize;
	int x, z;

public:
	Robot(Map* map);
	~Robot();

	void update(float deltaTime);

};
