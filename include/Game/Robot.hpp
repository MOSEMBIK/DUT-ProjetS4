#pragma once

#include <Game/Player.hpp>

class Robot: public Player
{
private:
	int mapSize;
	int x, z;

	std::vector<glm::ivec2> trajet;
	int case_of_t;

public:
	Robot(Map* map);
	virtual ~Robot() {}

	void update(float deltaTime);
	bool isPossible(glm::ivec2, glm::ivec2 aPos = glm::ivec2(-1, -1), std::list<glm::ivec2> &checked = std::list<glm::ivec2>());
	glm::ivec2 nearstSafeLoc();


	glm::ivec2 choseDestination(int mode = 0);
	glm::ivec2 shouldBomb();											// Return (-1, -1) si ne peut choisir

	std::vector<glm::ivec2> genTrajetMann(glm::ivec2);
	void setTrajet(std::vector<glm::ivec2>);
};
