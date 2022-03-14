#pragma once

#include <Game/Player.hpp>
#include <Game/Map.hpp>

class Human: public Player
{
private:

public:
	Human(Map* map);
  	virtual ~Human() {}

};
