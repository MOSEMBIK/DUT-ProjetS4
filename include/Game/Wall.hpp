#pragma once

#include <Game/Actor.hpp>

class Wall: public Actor
{
private:
	int type;
	int health;

	void onDestroy();

public:
	enum Type { Metal, Stone, Wood };
	Wall(Map* map, int type = Metal);

	inline int getType() { return type; }
	inline int getHealth() { return health; }
	void removeHealth();

};
