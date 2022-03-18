
#include <Game/Map.hpp>
#include <Game/Actor.hpp>
#include <Game/Player.hpp>
#include <algorithm>
#include <numeric>
#include <algorithm>
#include <iterator> 
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

Map::Map() : mapMaterial(*Shader::find("Base")), mapActor(this)
{
	Resource::loadTexture("assets/map_texture.png", mapTexture);
	Resource::loadTexture("assets/map_texture_specular.png", mapTextureSpecular);
	mapMaterial.setDiffuseTexture(mapTexture->m_id);
	mapMaterial.setSpecularTexture(mapTextureSpecular->m_id);

	mapActor.m_meshes = { &mapMesh };
	mapActor.m_materials = { mapMaterial };
	mapActor.getTransform().setPosition(vec3(-0.5f, -0.5f, -0.5f));
}

Map::~Map() {
	for (Actor* actor : actors) {
		if(actor != nullptr)
			delete actor;
	}
	
	for (auto wall : walls) {
		if(wall.second != nullptr)
			delete wall.second;
	}
}

string Map::getData() const {
	string data = "";
	data += "Map size: " + to_string(mapSize) + "|";
	data += "walls:{";
	for (auto wall : walls) {
		if (wall.second != nullptr)
			data += wall.second->getData() + ",";
	}
	data += "}|";

	return data;
}


void Map::generateMap(int size, int wallPercentage) {
	walls.clear();
	if (size % 2 == 0)
		size++;
	this->mapSize = size;;
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			ivec2 pos(i, j);
			/**
			 * @brief Metal Walls
			 */
			if (i == 0 || i == sizeMax || j == 0 || j == sizeMax || (i%2 == 0 && j%2 == 0)) {
				walls[pos] = new Wall(this, Wall::Type::Metal);
				walls[pos]->getTransform().setPosition(vec3(i, 0, j));
			}
			/**
			 * @brief Random Walls
			 */
			else if (rand() % 100 < wallPercentage) {
				walls[pos] = new Wall(this, (rand()%5 == 0) ? Wall::Type::Stone : Wall::Type::Wood);
				walls[pos]->getTransform().setPosition(vec3(i, 0, j));
			}
		}
	}
	calculateWallMesh();
}

void Map::addActor(Actor* actor) {
	actors.push_back(actor);
}

void Map::addPlayer(Player* player) {
	players.push_back(player);
}

void Map::addBomb(Bomb* bomb, glm::ivec2 pos) {
	bomb->getTransform().setPosition(glm::vec3(pos.x, 0, pos.y));
	bombs[pos] = bomb;
}
	
void Map::draw() {	
	for (auto player : players) {
		if (player != nullptr)
			player->draw();
	}

	for (auto bomb : bombs) {
		if (bomb.second != nullptr)
			bomb.second->draw();
	}

	mapActor.draw();
}

void Map::update(float deltaTime) {	
	cerr << "Updating Map..." << endl;

	cerr << "Generate edges map..." << endl;
	genEdgesMap();

	for (auto player : players) {
		cerr << "Updating Players..." << endl;
		if (player != nullptr)
			player->update(deltaTime);
	}

	for (auto bomb : bombs) {
		cerr << "Updating Bombs..." << endl;
		if (bomb.second != nullptr)
			bomb.second->update(deltaTime);
	}

	cerr << "Map Updated" << endl;
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
	bool wallUpdate = false;
	std::vector<glm::ivec2> touched;
	for (int i = x; i < x+range; ++i) { //Droite
		glm::ivec2 pos(i, z);
		Wall* m = this->walls[pos];

		if (m != nullptr) {
			m->removeHealth();
			wallUpdate = true;
			break;
		}
		touched.push_back(pos);

	}

	for (int i = x-1; i > x-range; --i) { //Gauche
		glm::ivec2 pos(i, z);
		Wall* m = this->walls[pos];

		if (m != nullptr) {
			m->removeHealth();
			wallUpdate = true;
			break;
		}
		touched.push_back(pos);
	}

	for (int i = z-1; i > z-range; --i) { //Haut
		glm::ivec2 pos(x, i);
		Wall* m = this->walls[pos];

		if (m != nullptr) {
			m->removeHealth();
			wallUpdate = true;
			break;
		}
		touched.push_back(pos);
	}

	for (int i = z; i < z+range; ++i) { //Bas
		glm::ivec2 pos(x, i);
		Wall* m = this->walls[pos];

		if (m != nullptr) {
			m->removeHealth();
			wallUpdate = true;
			break;
		}
		touched.push_back(pos);
	}

	list<Player*> playersToRemove;
	for (Player* player : players) {
		glm::ivec3 pos(player->getTransform().getPosition());
		if (std::find(touched.begin(), touched.end(), glm::ivec2(pos.x,pos.z)) != touched.end())
			playersToRemove.push_back(player);
	}
	for (Player* player : playersToRemove) {
		players.remove(player);
		delete player;
		player = nullptr;
	}
	if (wallUpdate) calculateWallMesh();
}

void Map::calculateWallMesh()
{
	using WallType = Wall::Type;

	std::vector<Vertex> vertices;
	for(int x = 0; x < mapSize; x++)
	{
		for(int z = 0; z < mapSize; z++)
		{
			auto iterat = walls.find(ivec2(x,z));
			if(iterat == walls.end() || iterat->second == nullptr)
			{
				vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 1, 0), vec2(0, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 1, 0), vec2(0.5f, 0)});
				vertices.push_back(Vertex { vec3(x, 0, z), vec3(0, 1, 0), vec2(0, 0)});
				
				vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 1, 0), vec2(0, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 1, 0), vec2(0.5f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 1, 0), vec2(0.5f, 0)});
			}
			else
			{
				WallType type = iterat->second->getType();
				vec2 uv;
				switch(type)
				{
					default:
						uv = glm::vec2(0.0f, 0.0f);
						break;

					case WallType::Wood:
						uv = glm::vec2(0.5f, 0.0f);
						break;
						
					case WallType::Stone:
						uv = glm::vec2(0.0f, 0.5f);
						break;

					case WallType::Metal:
						uv = glm::vec2(0.5f, 0.5f);
						break;
				}

				vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.0f)});
				vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.0f)});
				
				vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.0f)});
				
				if(auto it = walls.find(ivec2(x + 1,z)); it == walls.end() || it->second == nullptr)
				{
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.5f)});
				}

				if(auto it = walls.find(ivec2(x - 1,z)); it == walls.end() || it->second == nullptr)
				{
					vertices.push_back(Vertex { vec3(x, 0, z), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
				}

				if(auto it = walls.find(ivec2(x,z + 1)); it == walls.end() || it->second == nullptr)
				{
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.5f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.5f)});
				}

				if(auto it = walls.find(ivec2(x,z - 1)); it == walls.end() || it->second == nullptr)
				{
					vertices.push_back(Vertex { vec3(x, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.0f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.5f)});
				}
			}
		}
	}
	mapMesh = Mesh(vertices);
	mapActor.m_meshes = { &mapMesh };
}

///-------------------------------------------------------
///--- Fonctions utiles au déplacement des Players

std::vector<glm::ivec2> Map::nearRoads(glm::ivec2 coord) {
	std::vector<glm::ivec2> nearR;

	glm::ivec2 cordTest (coord[0]-1, coord[1]);
	if (whatIs(cordTest) == 0) nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0], coord[1]-1);
	if (whatIs(cordTest) == 0) nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0]+1, coord[1]);
	if (whatIs(cordTest) == 0) nearR.push_back(cordTest);
	cordTest = glm::ivec2(coord[0], coord[1]+1);
	if (whatIs(cordTest) == 0) nearR.push_back(cordTest);
	return nearR;
}

void Map::genEdgesMap(){
	std::vector<glm::ivec2> roads;
	for (int i=1; i < mapSize; i++) {
		for (int j=1; j < mapSize; j++) {
			glm::ivec2 checkCoord (i, j);
			if (whatIs(checkCoord) == 0) roads.push_back(checkCoord);
		}
	}
	for (glm::ivec2 rCase : roads) {
		std::vector<glm::ivec2> nRC = nearRoads(rCase);
		edges_map[rCase] = nRC;
	}
}

bool Map::isReachable(glm::ivec2 coord){
	if (whatIs(coord) == 0) return true;

	return false;
}

int Map::whatIs(glm::ivec2 coord){
	if (walls[coord] != nullptr) return 1;
	if (bombs[coord] != nullptr) return 2;

	return 0;
}

std::list<glm::ivec2> Map::getPlayersMap(){
	std::list<glm::ivec2> lst;
	for (Player* ply : players){
		lst.push_back(glm::ivec2( int(ply->getTransform().getPosition().x), int(ply->getTransform().getPosition().z)));
	}
	return lst;
}

std::map<glm::ivec2, float, cmpVec> Map::getDangerMap(){
	std::map<glm::ivec2, float, cmpVec> danger;
	if (bombs.size() >= 1) {
		for (std::pair<glm::ivec2, Bomb*> bombP : bombs){
			if (bombP.second != nullptr) {
				for (int i=0; i <= bombP.second->getRange(); i++){
					danger[glm::ivec2 (bombP.first[0]+i, bombP.first[1])] = bombP.second->getTimer()/2;
					danger[glm::ivec2 (bombP.first[0], bombP.first[1]+i)] = bombP.second->getTimer()/2;
					danger[glm::ivec2 (bombP.first[0]-i, bombP.first[1])] = bombP.second->getTimer()/2;
					danger[glm::ivec2 (bombP.first[0], bombP.first[1]-i)] = bombP.second->getTimer()/2;
				}
			}
		}
	}
	return danger;
}

///-------------------------------------------------------