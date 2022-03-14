
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
			 * @brief Border Walls
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

///-------------------------------------------------------
///--- Fonctions utiles au déplacement des Players

std::vector<glm::ivec2> Map::nearRoads(glm::ivec2 coord) {
	std::vector<glm::ivec2> nearR;

	glm::ivec2 cordTest (coord[0]-1, coord[1]);
	if (whatIs(cordTest) == "void") nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0], coord[1]-1);
	if (whatIs(cordTest) == "void") nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0]+1, coord[1]);
	if (whatIs(cordTest) == "void") nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0], coord[1]+1);
	if (whatIs(cordTest) == "void") nearR.push_back(cordTest);
}

void Map::genEdgesMap(){
	std::vector<glm::ivec2> roads;
	for (int i=1; i < mapSize; i++) {
		for (int j=1; j < mapSize; j++) {
			glm::ivec2 checkCoord (i, j);
			if (whatIs(checkCoord) == "void") roads.push_back(checkCoord);
		}
	}
	for (glm::ivec2 rCase : roads) {
		std::vector<glm::ivec2> nRC = nearRoads(rCase);
		edges_map[rCase] = nRC;
	}
}

bool Map::isReachable(glm::ivec2 coord){
	if (whatIs(coord) == "void") return true;

	return false;
}

std::string Map::whatIs(glm::ivec2 coord){
	if (walls[coord] != nullptr) return "wall";
	if (bombs[coord] != nullptr) return "bomb";

	return "void";
}

///-------------------------------------------------------