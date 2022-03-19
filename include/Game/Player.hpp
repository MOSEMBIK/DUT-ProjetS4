#pragma once

#include <Game/Actor.hpp>
#include <Game/Bomb.hpp>

#include <glm/vec3.hpp>

class Player: public Actor
{
private:
	std::vector<Bomb> launchedBombs;
	glm::vec3 color;
	float speed = 1.0f;
	int bombRange = 1;
	int bombCount = 1;

protected:
	unsigned char movement = '0';
	int mapSize;
	int x, z;

public:
	Player(Map* map);
	Player(Map* map, std::string& data);
	virtual ~Player() {};

	std::string getData() const; // Fonction qui retourne les données du joueur sous forme de string
	std::string getPosRot() const; // Retourne les positions et les rotations du joueur (pour le serveur)
	void loadPosRot(glm::vec3 pos, glm::vec3 rot); // Charge les positions et les rotations du joueur (pour le client)

	inline float getSpeed() const { return speed; }

	void setBomb(glm::ivec2);

	void update(float deltaTime);

	void upBombCount();

	void upBombRange();
};
