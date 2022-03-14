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
	std::vector<Player*> players;
	int mapSize;

public:
	Map();
	~Map();

	void generateMap(int size = 13, int wallPercentage = 90);
	void addActor(Actor* actor);
	void addPlayer(Player* player);
	void addBomb(Bomb*, glm::ivec2);

	inline void removeWall(glm::ivec2 pos) { walls[pos] = nullptr; }
	inline void removeBomb(glm::ivec2 pos) { bombs[pos] = nullptr; }

	//void removePlayer()

	inline int getSize() { return mapSize; }

	void draw();
	void update(float deltaTime);

	void onExplosion(int, int, int);
	


};
