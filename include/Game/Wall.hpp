#pragma once

#include <Game/Actor.hpp>

class Wall: public Actor
{
private:
	int type;
	int health;

	void onDestroy();

public:
	Wall();


};
