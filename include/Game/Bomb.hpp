#pragma once

#include <Game/Actor.hpp>

#include <glm/vec3.hpp>

class Bomb: public Actor
{
private:
	glm::vec3 color;
	int range = 1;
	float duration = 1.0f;
	float timer = 2.0f;
	bool exploded = false;

public:
	Bomb(Map* map, glm::vec3 color, int range = 2, float duration = 1.0f);
	virtual ~Bomb() {}
	void onExplode();

	virtual void update(float deltaTime);

};
