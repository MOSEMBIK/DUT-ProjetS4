#pragma once

#include <Game/Actor.hpp>

#include <glm/vec3.hpp>

class Bomb: public Actor
{
private:
	glm::vec3 color;
	int range;
	int duration;
	int timer;

public:
	Bomb();


};
