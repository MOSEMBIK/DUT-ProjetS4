#pragma once

#include <Game/Actor.hpp>
#include <Game/Wall.hpp>

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
	unsigned char mapSize;

public:
	Map();
	~Map();

	void generateMap(unsigned char size = 13, unsigned char wallPercentage = 90);
	void addActor(Actor* actor);

	inline int getSize() { std::cerr << mapSize << std::endl; return mapSize; }

	void draw();
	void update(float deltaTime);
};
