#pragma once

#include <Game/Player.hpp>
#include <Game/Map.hpp>

class Human: public Player
{
private:
	unsigned int keyPressed = 0;
	bool spacePressed = false;

public:
	Human(Map* map);
  	virtual ~Human() {}

	void update(float deltaTime);

};
