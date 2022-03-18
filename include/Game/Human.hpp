#pragma once

#include <Game/Player.hpp>
#include <Game/Map.hpp>

class Human: public Player
{
private:
	int mapSize;
	int x, z;
public:
	Human(Map* map);
  	virtual ~Human() {}

	void update(float deltaTime);

};
