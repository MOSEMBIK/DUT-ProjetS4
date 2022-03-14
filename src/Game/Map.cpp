
#include <Game/Map.hpp>
#include <Game/Actor.hpp>
#include <Game/Player.hpp>
#include <algorithm>

using namespace std;

Map::Map() {}

Map::~Map() {
	for (Actor* actor : actors) {
		delete actor;
	}
	
	for (auto wall : walls) {
		delete wall.second;
	}
}

void Map::generateMap(int size, int wallPercentage) {
	if (size % 2 == 0)
		size++;
	this->mapSize = size;;
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			glm::ivec2 pos(i, j);
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
			else if (rand() % 100 < wallPercentage) {
				walls[pos] = new Wall(this, (rand()%5 == 0) ? Wall::Type::Stone : Wall::Type::Wood);
				walls[pos]->getTransform().setPosition(glm::vec3(i, 0, j));
			}
		}
	}
}

void Map::addActor(Actor* actor) {
	actors.push_back(actor);
}

void Map::addPlayer(Player* player) {
	players.push_back(player);
}

void Map::draw() {
	for (auto wall : walls) {
		if (wall.second != nullptr)
			wall.second->draw();
	}
	
	for (Actor* actor : actors) {
		if (actor != nullptr)
			actor->draw();
	}
}

void Map::update(float deltaTime) {
	for (auto wall : walls) {
		if (wall.second != nullptr) 
			wall.second->update(deltaTime);
	}
	
	for (Actor* actor : actors) {
		if (actor != nullptr)
			actor->update(deltaTime);
	}
	
}

/**
 * @brief Quand une bombe explose sur la map, onExplosion crée un tableau des cases touchées. 
 * Ensuite, elle vérifie si un joueur se situe dans une de ses cases
 * 
 * @param x 
 * @param z 
 * @param range 
 */
void Map::onExplosion(int x, int z, int range) {
	std::vector<glm::vec3> touched;
	for (int i = x; i < x+range; ++i) { //Droite
		glm::vec3 pos(i, 0, z);
		Wall* m = this->walls[pos];

		std::cout << m->getType() << std::endl;

		if (m != nullptr) {
			m->removeHealth();
			break;
		}
		touched.push_back(pos);

	}

	for (int i = x; i > x-range; --i) { //Gauche
		glm::vec3 pos(i, 0, z);
		Wall* m = this->walls[pos];

		std::cout << m->getType() << std::endl;

		if (m != nullptr) {
			m->removeHealth();
			break;
		}
		touched.push_back(pos);
	}

	for (int i = z; i < z-range; --z) { //Haut
		glm::vec3 pos(i, 0, z);
		Wall* m = this->walls[pos];

		std::cout << m->getType() << std::endl;

		if (m != nullptr) {
			m->removeHealth();
			break;
		}
		touched.push_back(pos);
	}

	for (int i = z; i > z+range; ++z) { //Bas
		glm::vec3 pos(i, 0, z);
		Wall* m = this->walls[pos];

		std::cout << m->getType() << std::endl;

		if (m != nullptr) {
			m->removeHealth();
			break;
		}
		touched.push_back(pos);
	}

	for (Player* player : players) {
		if (std::find(touched.begin(), touched.end(), player->getTransform().getPosition()) != touched.end()) {
			player = nullptr;
			delete player;
		}
	}
}


void Map::removeWall(glm::ivec2 pos) {
	this->walls[pos] = nullptr;
}