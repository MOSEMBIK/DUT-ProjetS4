#pragma once

#include <Game/Actor.hpp>
#include <Game/Wall.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <unordered_map>

struct cmpVec {
    bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const {
        return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
    }
};

class Map
{
private:
	std::vector<Actor*> actors;
	std::map<glm::ivec2, Wall*, cmpVec> walls;
	std::map<glm::ivec2, Bomb*, cmpVec> bombs;

	int mapSize;
public:
	std::unordered_map<glm::ivec2, std::vector<glm::ivec2>> edges_map; 			// Graph des accessibilités pour chaque position
	

public:
	Map();
	~Map();

	void generateMap(int size = 13);
	void addActor(Actor* actor);

	inline int getSize() { return mapSize; }

	void draw();
	void update(float deltaTime);

	///--- Fonctions utiles au déplacement des Players
	std::vector<glm::ivec2> nearRoads(glm::ivec2);								// Retourne le vecteur de toute les coordonées voisine accessible depuis celles données
	void genEdgesMap();															// (Re)genere la edges map
	bool isReachable(glm::ivec2);												// Retourne true si le localisation passee est accessible
	std::string whatIs(glm::ivec2);												// Retourne le type de l'Actor aux coordonnees passees

};
