
#include <Game/Map.hpp>
#include <Game/Actor.hpp>

using namespace std;

Map::Map() {}

Map::~Map() {
	for (auto& actor : actors) {
		delete actor;
	}
	
	for (auto& wall : walls) {
		delete wall.second;
	}
}

void Map::generateMap(int size) {
	if (size % 2 == 0) {
		size++;
	}
	this->mapSize = size;
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			glm::ivec2 pos = glm::ivec2(i, j);

			/**
			 * @brief Metal Walls
			 */
			if (i == 0 || i == sizeMax || j == 0 || j == sizeMax || (i%2 == 0 && j%2 == 0)) {
				walls[pos] = new Wall(this, Wall::Type::Metal);
				walls[pos]->getTransform().setPosition(glm::vec3(i, 0, j));
			}

			/**
			 * @brief Random Walls
			 */
			else if (rand() % 10 <= 6) {
				walls[pos] = new Wall(this, (rand()%5 == 0) ? Wall::Type::Stone : Wall::Type::Wood);
				walls[pos]->getTransform().setPosition(glm::vec3(i, 0, j));
			}
		}
	}
}

void Map::addActor(Actor* actor) {
	actors.push_back(actor);
}

void Map::draw() {
	for (auto wall : walls) {
		wall.second->draw();
	}
	
	for (Actor* actor : actors) {
		actor->draw();
	}
}

void Map::update(float deltaTime) {
	for (auto wall : walls) {
		wall.second->update(deltaTime);
	}
	
	for (Actor* actor : actors) {
		actor->update(deltaTime);
	}
}
