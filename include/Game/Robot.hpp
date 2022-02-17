#pragma once

#include <Game/Player.hpp>

class Robot: public Player
{
private:
	int x = rand()%13, z = rand()%13;

public:
	Robot();

	void update(float deltaTime);

};
