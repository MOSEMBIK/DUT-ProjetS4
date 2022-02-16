
#include <Game/Map.hpp>
#include <Game/Actor.hpp>

Map::Map() {}

void Map::generateMap(int size) {
	if (size % 2 == 0) {
		size++;
	}
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			glm::ivec2 pos = glm::ivec2(i, j);

			/**
			 * @brief Border Walls
			 */
			if (i == 0 || i == sizeMax || j == 0 || j == sizeMax || (i%2 == 0 && j%2 == 0)) {
				walls[pos] = new Wall(Wall::Type::Metal);
				walls[pos]->getTransform().SetPosition(glm::vec3(i, j, 0));
				walls[pos]->getTransform().SetScale(glm::vec3(0.5f));
			}

			/**
			 * @brief Random Walls
			 * 
			 */
			else if (rand() % 10 <= 6) {
				walls[pos] = new Wall((rand()%5 == 0) ? Wall::Type::Stone : Wall::Type::Wood);
				walls[pos]->getTransform().SetPosition(glm::vec3(i, j, 0));
				walls[pos]->getTransform().SetScale(glm::vec3(0.5f));
			}
		}
	}
}

void Map::Draw() {
	this->draw();
	for (auto wall : walls) {
		wall.second->draw();
	}
	for (Actor* actor : actors) {
		actor->draw();
	}
}
