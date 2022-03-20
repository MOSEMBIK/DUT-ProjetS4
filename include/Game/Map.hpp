#pragma once

#include <Game/Actor.hpp>
#include <Game/Wall.hpp>
#include <Game/Player.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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
	std::list<Player*> players;

	Actor mapActor;
	Mesh mapMesh;
	Material mapMaterial;
	Texture* mapTexture;
	Texture* mapTextureSpecular;

	int mapSize;
  
public:
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges_map; 			// Graph des accessibilités pour chaque position
	std::map<glm::ivec2, float, cmpVec> danger_map; 							// Graph des zones dangeureuses

  
public:
	Map();
	~Map();

	void generateMap(int size = 13, int wallPercentage = 90);
	void addActor(Actor* actor);
	void addPlayer(Player* player);
	void addBomb(Bomb*, glm::ivec2);

	inline void removeWall(glm::ivec2 pos) { walls.erase(walls.find(pos)); }
	inline void removeBomb(glm::ivec2 pos) { bombs.erase(bombs.find(pos)); }
	inline void removePlayer(Player* play) { players.remove(play); }

	//void removePlayer()

	inline int getSize() { return mapSize; }

	void draw();
	void calculateWallMesh();
	void update(float deltaTime);

	void onExplosion(int, int, int);
	

	///--- Fonctions utiles au déplacement des Players
	std::vector<glm::ivec2> nearRoads(glm::ivec2);								// Retourne le vecteur de toute les coordonées voisine accessible depuis celles données
	void genEdgesMap();															// (Re)genere la edges map
	void genDangerMap();														// (Re)genere la danger map
	bool isReachable(glm::ivec2);												// Retourne true si le localisation passee est accessible
	int whatIs(glm::ivec2);														// Retourne le type de l'Actor aux coordonnees passees

	std::list<std::pair<glm::ivec2, int>>  getPlayersMap();						// Retourne la liste des positions des Players
};
