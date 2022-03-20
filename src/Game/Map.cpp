
#include <Game/Map.hpp>
#include <Game/Actor.hpp>
#include <Game/Human.hpp>
#include <Game/Player.hpp>
#include <Game/ObjectPerk.hpp>
#include <algorithm>
#include <numeric>
#include <algorithm>
#include <iterator> 
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

Map::Map() : mapActor(this), mapMaterial(Shader::find("Base"))
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
	string data = "#loadMap " + to_string(mapSize) + "|";

	for (auto wall : walls) {
		if (wall.second != nullptr)
			data += wall.second->getData() + ";";
	}
	data += "|";
	for (auto player : players) {
		if (player != nullptr)
			data += player->getData() + ";";
	}
	data += "|";

	return data;
}

void Map::loadMap(const std::string& mapData, int humanId) {
	cerr << endl << "Loading map from string.." << endl; float time = glfwGetTime();

	for (auto wall : walls) {
		if (wall.second != nullptr)
			delete wall.second;
	} walls.clear();
	for (auto bomb : bombs) {
		if (bomb.second != nullptr)
			delete bomb.second;
	} bombs.clear();
	for (auto player : players) {
		if (player != nullptr)
			delete player;
	} players.clear();
	for (auto bonus : bonuses) {
		if (bonus.second != nullptr)
			delete bonus.second;
	} bonuses.clear();


	// Parsing map size
	int pos = mapData.find("|");
	string sizeStr = mapData.substr(0, pos);
	this->mapSize = stoi(sizeStr);

	// Parsing walls
	pos = mapData.find("|", pos);
	int pos2 = mapData.find("|", pos + 1);
	string wallsStr = mapData.substr(pos + 1, pos2 - pos - 2);
	vector<string> wallsData;
	for (int i=0; i < (int)wallsStr.size(); i++) {
		if (wallsStr[i] == ';') {
			wallsData.push_back(wallsStr.substr(0, i));
			wallsStr = wallsStr.substr(i + 1, wallsStr.size() - i - 1);
			i = 0;
		}
	} wallsData.push_back(wallsStr);

	for (auto wall : wallsData) {
		wall = wall.substr(1, wall.size() - 2);
		Wall* w = new Wall(this, wall);
		ivec2 pos(w->getTransform().getPosition().x, w->getTransform().getPosition().z);
		walls[pos] = w;
	}
	calculateWallMesh();

	// Parsing players
	pos = mapData.find("|", pos2);
	string playersStr = mapData.substr(pos2 + 1, pos - pos2 - 3);
	vector<string> playersData;
	for (int i=0; i < (int)playersStr.size(); i++) {
		if (playersStr[i] == ';') {
			playersData.push_back(playersStr.substr(0, i));
			playersStr = playersStr.substr(i + 1, playersStr.size() - i - 1);
			i = 0;
		}
	}
	for (auto player : playersData) {
		player = player.substr(1, player.size() - 2);
		if (humanId != stoi(player.substr(player.find_last_of(",") + 1, player.size())))
			players.push_back(new Player(this, player));
		else
			players.push_back(new Human(this, player));
	}

	cerr << "Loaded map from string in " << (glfwGetTime() - time) * 1000 << "ms" << endl;
}

string Map::getPosRot() const {
	string data = "#updatePosRot ";
	for (auto player : players) {
		if (player != nullptr)
			data += player->getPosRot() + ";";
	}
	return data;
}

string Map::getPlayerData(int id) const {
	for (auto player : players) {
		if (player != nullptr && player->getId() == id)
			return player->getData();
	}
	return "";
}

Player* Map::getPlayer(int id) const {
	for (auto player : players) {
		if (player != nullptr && player->getId() == id)
			return player;
	}
	return nullptr;
}

void Map::loadPosRot(const std::string& posRotData) {
	string playersStr = posRotData;
	vector<string> playersData;
	for (int i=0; i < (int)playersStr.size(); i++) {
		if (playersStr[i] == ';') {
			playersData.push_back(playersStr.substr(0, i));
			playersStr = playersStr.substr(i + 1, playersStr.size() - i - 1);
			i = 0;
		}
	}
	for (auto player : playersData) {
		vector<float> playerData;
		for (int i = 0; i < (int)player.size(); i++) {
			if (player[i] == ',') {
				try { playerData.push_back(stof(player.substr(0, i))); } catch (...) {}
				player = player.substr(i + 1, player.size() - i - 1);
				i = 0;
			}
		} playerData.push_back(stof(player));
		for (auto p : players) {
			if (p != nullptr && p->getId() == int(playerData[0])) {
				p->loadPosRot(
					vec3(playerData[1], playerData[2], playerData[3]),
					vec3(playerData[4], playerData[5], playerData[6]),
					playerData[7], playerData[8]
				);
				break;
			}
		}
	}
}

void Map::loadBombs(const std::string& bombsData) {
	string bombsStr = bombsData;
	for (int i=0; i < (int)bombsStr.size(); i++) {
		if (bombsStr[i] == ';') {
			string bomb = bombsStr.substr(0, i);
			vector<string> bombData;
			for (int i=0; i < (int)bomb.size(); i++) {
				if (bomb[i] == ',') {
					bombData.push_back(bomb.substr(0, i));
					bomb = bomb.substr(i + 1, bomb.size() - i - 1);
					i = 0;
				}
			} bombData.push_back(bomb);
			int x = stoi(bombData[0]);
			int z = stoi(bombData[1]);
			vec3 color = vec3(stof(bombData[2]), stof(bombData[3]), stof(bombData[4]));
			float range = stof(bombData[5]);

			addBomb(new Bomb(this, color, range), glm::ivec2(x, z));
			bombsStr = bombsStr.substr(i + 1, bombsStr.size() - i - 1);
			i = 0;
		}
	}
}

void Map::movePlayer(int id, int x, int z) {
	for (auto player : players) {
		if (player != nullptr && player->getId() == id) {
			player->move(x, z);
			return;
		}
	}
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
	glm::ivec3 posn = player->getTransform().getPosition();
	glm::ivec2 pos = glm::ivec2(posn.x,posn.z);
	glm::ivec2 pos1 = glm::ivec2(posn.x+1,posn.z);
	glm::ivec2 pos2 = glm::ivec2(posn.x,posn.z+1);
	glm::ivec2 pos3 = glm::ivec2(posn.x-1,posn.z);
	glm::ivec2 pos4 = glm::ivec2(posn.x,posn.z-1);
	std::vector<glm::ivec2> positions;
	positions.push_back(pos);
	positions.push_back(pos1);
	positions.push_back(pos2);
	positions.push_back(pos3);
	positions.push_back(pos4);
	for (glm::ivec2 pos : positions){
		if (walls[pos] != nullptr && walls[pos]->getType() != Wall::Type::Metal){
			delete walls[pos];
			walls[pos] = nullptr;
		}
	}
}

void Map::addBomb(Bomb* bomb, glm::ivec2 pos) {
	bomb->getTransform().setPosition(glm::vec3(pos.x, 0, pos.y));
	bombs[pos] = bomb;
}

void Map::addBonus(ObjectPerk* bonus, glm::ivec2 pos) {
	bonuses[pos] = bonus;
}

ObjectPerk::Type Map::pickUpBonus(glm::ivec2 pos) {
	ObjectPerk::Type type = ObjectPerk::Type::None;
	if (bonuses[pos] != nullptr) {
		type = bonuses[pos]->getType();
		delete bonuses[pos];
		bonuses[pos] = nullptr;
	}
	return type;
}

/**
 * @brief Renvoie une position possible sur la map collée à la bordure de la map
 * 
 * @param int
 * 
 * @return glm::ivec2
 */
glm::ivec2 Map::choosePos(int iterateur) {
	glm::ivec2 pos(1, 1);
	int diviseur = iterateur / 4 + 1;
	int size = (mapSize - 3);

	switch (iterateur % 4) {
	case 0:
		pos.y += size - size / diviseur;
		break;
	case 1:
		pos.x += size / diviseur;
		break;
	case 2:
		pos = glm::ivec2(size+1, size+1);
		pos.y -= size - size / diviseur;
		break;
	case 3:
		pos = glm::ivec2(1, size+1);
		pos.x += size - size / diviseur;
		break;
	}

	return pos;
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

	for (auto bonus : bonuses) {
		if (bonus.second != nullptr)
			bonus.second->draw();
	}

	for (auto bombExplosion : bombsExplosions) {
		if(bombExplosion != nullptr)
			bombExplosion->draw(false);
	}

	mapActor.draw();
}

void Map::update(float deltaTime) {
	genEdgesMap();

	for (auto player : players) {
		if (player != nullptr)
			player->update(deltaTime);
	}

	for (auto bomb : bombs) {
		if (bomb.second != nullptr)
			bomb.second->update(deltaTime);
	}

	std::list<BombExplosion*> exps;
	for (auto be : bombsExplosions)
	{
		if(be->m_time <= 0)
			exps.push_back(be);
	}
	for (auto exp : exps)
	{
		bombsExplosions.remove(exp);
		delete exp;
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
	bool wallUpdate = false;
	std::vector<glm::ivec2> touched;
	for (int i = x; i < x+range; ++i) { //Droite
		glm::ivec2 pos(i, z);
		Wall* m = this->walls[pos];
		bombsExplosions.push_back(new BombExplosion(this, glm::vec3(pos.x, 0, pos.y)));
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
		bombsExplosions.push_back(new BombExplosion(this, glm::vec3(pos.x, 0, pos.y)));
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
		bombsExplosions.push_back(new BombExplosion(this, glm::vec3(pos.x, 0, pos.y)));
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
		bombsExplosions.push_back(new BombExplosion(this, glm::vec3(pos.x, 0, pos.y)));
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