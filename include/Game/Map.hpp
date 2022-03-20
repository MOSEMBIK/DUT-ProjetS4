#pragma once

#include <Game/Actor.hpp>
#include <Game/Wall.hpp>
#include <Game/Player.hpp>
#include <Game/ObjectPerk.hpp>
#include <Game/BombExplosion.hpp>

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
	std::list<BombExplosion*> bombsExplosions;
	std::map<glm::ivec2, ObjectPerk*, cmpVec> bonuses;

	Actor mapActor;
	Mesh mapMesh;
	Material mapMaterial;
	Texture* mapTexture;
	Texture* mapTextureSpecular;

	int mapSize;
  
public:
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges_map; 			// Graph des accessibilités pour chaque position

  
public:
	Map();
	~Map();

	// Multiplayer
	std::string getData() const; // Fonction qui retourne les données de la map sous forme de string
	std::string getPosRot() const; // Retourne les positions et les rotations des joueurs
	std::string getPlayerData(int id) const; // Retourne les données d'un joueur
	Player* getPlayer(int id) const; // Retourne un joueur
	void loadMap(const std::string& mapData, int humanId); // Fonction qui charge la map à partir d'un string
	void loadPosRot(const std::string& posRotData); // Charge les positions et les rotations des joueurs
	void loadBombs(const std::string& bombsData); // Charge les nouvelles bombes reçues
	void movePlayer(int id, int x, int z); // Déplace un joueur selon son ID

	void generateMap(int size = 13, int wallPercentage = 90);
	void addActor(Actor* actor);
	void addPlayer(Player* player);
	void addBomb(Bomb*, glm::ivec2);
	void addBonus(ObjectPerk* bonus, glm::ivec2);

	inline void removeWall(glm::ivec2 pos) { walls[pos] = nullptr; }
	inline void removeBomb(glm::ivec2 pos) { bombs[pos] = nullptr; }
	inline void removePlayer(Player* play) { players.remove(play); }
	ObjectPerk::Type pickUpBonus(glm::ivec2 pos);

	glm::ivec2 choosePos(int iterateur = 0);

	//void removePlayer()

	inline int getSize() { return mapSize; }

	void draw();
	void calculateWallMesh();
	void update(float deltaTime);

	void onExplosion(int, int, int, bool);
	

	///--- Fonctions utiles au déplacement des Players
	std::vector<glm::ivec2> nearRoads(glm::ivec2);								// Retourne le vecteur de toute les coordonées voisine accessible depuis celles données
	void genEdgesMap();															// (Re)genere la edges map
	bool isReachable(glm::ivec2);												// Retourne true si le localisation passee est accessible
	int whatIs(glm::ivec2);														// Retourne le type de l'Actor aux coordonnees passees

	std::list<glm::ivec2> getPlayersMap();										// Retourne la liste des positions des Players
	std::map<glm::ivec2, float, cmpVec> getDangerMap();							// Retourne la map complete avec pour chaque coord un valeur allant de 0 à 1 (1->safe, 0->danger)

};
