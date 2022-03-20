#pragma once

#include <Game/Actor.hpp>

#include <glm/vec3.hpp>

class Bomb: public Actor
{
private:
	glm::vec3 color;
	int range = 2;
	float duration = 1.0f;
	float timer = 2.0f;
	bool exploded = false;

public:
	Bomb(Map* map, glm::vec3 color, int range = 2);
	virtual ~Bomb() {}
	int getRange() {return range;}
	float getTimer() {return timer;}

	void onExplode(bool);

	virtual void update(float deltaTime);

};
