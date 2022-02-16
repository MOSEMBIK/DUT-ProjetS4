
#include <Game/Map.hpp>
#include <Game/Actor.hpp>

Map::Map() {}

void Map::generateMap(int size) {
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			glm::ivec2 pos = glm::ivec2(i, j);
			if (i == 0 || i == sizeMax || j == 0 || j == sizeMax) {
				walls[pos] = new Wall();
				walls[pos]->getTransform().SetPosition(glm::vec3(i, 0, j));
				walls[pos]->getTransform().SetScale(glm::vec3(0.05f));
			}
		}
	}
}

void Map::draw() {
	for (auto wall : walls) {
		wall.second->draw();
	}
	for (Actor* actor : actors) {
		actor->draw();
	}
}
