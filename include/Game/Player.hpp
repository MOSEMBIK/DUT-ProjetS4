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
	int bombRange = 2;
	int bombCount = 1;

protected:
	unsigned char movement = '0';
	int mapSize;
	int x, z;

public:
	Player(Map* map, glm::ivec2 pos);
	virtual ~Player() {};

	// Multiplayer
	Player(Map* map, glm::ivec2 pos, int playerId);
	Player(Map* map, std::string& data);
	std::string getData() const; // Fonction qui retourne les données du joueur sous forme de string
	std::string getPosRot() const; // Retourne les positions et les rotations du joueur (pour le serveur)
	void loadPosRot(glm::vec3 pos, glm::vec3 rot, int x, int z); // Charge les positions et les rotations du joueur (pour le client)
	void move(int x, int z); // Déplace le joueur selon ses coordonnées

	inline float getSpeed() const { return speed; }

	void setBomb(glm::ivec2);

	void update(float deltaTime);

};
