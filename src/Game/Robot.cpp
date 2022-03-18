#include <Game/Robot.hpp>
#include <Game/Map.hpp>

Robot::Robot(Map* map) : Player(map) {
	this->mapSize = map->getSize() - 1;
	this->x = rand() % mapSize;
	this->z = rand() % mapSize;
}

void Robot::update(float deltaTime) {
	case_of_t++;
	x = trajet[case_of_t][0];
	z = trajet[case_of_t][1];

	Transform& transform = getTransform();
	glm::vec3 pos = transform.getPosition();
	if (int(pos.x) == x && int(pos.z) == z) {
		x = rand() % mapSize;
		z = rand() % mapSize;
	}

	glm::quat targetRotation = transform.getRotation();
	if (int(pos.x) < x) {
		transform.translate(glm::vec3(1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(180.0f), 0));
	}
	else if (int(pos.x) > x) {
		transform.translate(glm::vec3(-1.0f, 0, 0) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, 0, 0));
	}
	else if (int(pos.z) < z) {
		transform.translate(glm::vec3(0, 0, 1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(90.0f), 0));
	}
	else if (int(pos.z) > z) {
		transform.translate(glm::vec3(0, 0, -1.0f) * deltaTime * (2 + getSpeed()));
		targetRotation = glm::quat(glm::vec3(0, glm::radians(-90.0f), 0));
	}
	transform.setRotation(glm::slerp(transform.getRotation(), targetRotation, 6.0f / 60.0f));
}

///--- REFLEXION
// Destination
/**
 * @brief Check if aPos is reachable from coord
 * 
 * @param coord Destination location
 * @param aPos Start location
 * @param checked List for recursivity that keep in mind checked locations
 * @return Boolean (true->case is reachable, false->case isn't reachable)
 */
bool Robot::isPossible(glm::ivec2 coord, glm::ivec2 aPos, std::list<glm::ivec2> &checked){
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;
	glm::ivec2 pos;
	if (aPos[0] == -1 && aPos[1] == -1) pos = trajet[case_of_t];
	else  pos = aPos;

	checked.push_back(pos);
	for (glm::ivec2 next : edges[pos]) {
		if (find(checked.begin(), checked.end(), next) == checked.end()){
			if (next[0] == coord[0] || next[1] == coord[1]) {
				return true;
			} else {
				return isPossible(coord, next, checked);
			}
		}
	}
	return false;
}

/**
 * @brief Think about the best location to go 
 * 
 * @return Chosen location
 */
glm::ivec2 Robot::choseDestination(){
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;
	std::list<glm::ivec2> players = map->getPlayersMap();
	std::pair<glm::ivec2, std::pair<int, float>> bombs = map->getBombsMap();
	glm::ivec2 pos = trajet[case_of_t];

	// Nearst Player location
	glm::ivec2 nearstPlayer = players.front();
	for (glm::ivec2 ply : players) if (sqrt(abs(ply[0] - pos[0]) + abs(ply[1] - pos[1])) < sqrt(abs(nearstPlayer[0] - pos[0]) + abs(nearstPlayer[1] - pos[1]))) nearstPlayer = ply;
	glm::ivec2 dest = nearstPlayer;
	if (isPossible(dest)) return dest;

	// Explosion safe location
	
}

// Bombs
/**
 * @brief Return were the Robot can place a Bomb
 * @return 0->no, 1->up, 2->right, 3->bot, 4->left 
 */
int shouldBomb(){
	
}

///--- DEPLACEMENTS
/**
 * @brief Mannathan A* Algorithm
 * @param destination Destination location
 * @return Vector of coords that represent the best way found to reach destination
*/
std::vector<glm::ivec2> Robot::genTrajetMann(glm::ivec2 destination) {
	std::vector<glm::ivec2> new_trajet;
	new_trajet.push_back(trajet[case_of_t]);

	int done = 0;

	if (map->isReachable(destination)){
		
		std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;

		if (map->whatIs(new_trajet[done]) != 0){
			new_trajet.push_back(edges[new_trajet[done]][0]);
			done += 1;
		}

		std::vector<std::pair<glm::ivec2, int>> queue;
		queue[0] = std::pair<glm::ivec2, int> (new_trajet.back(), 0);

		std::map<glm::ivec2, int, cmpVec> clout;
		clout[new_trajet[done]] = 0;

		// Génération plus court chemin
		while (queue.empty()) {

			// Recuperation de la case optimale
			int idx = 0;
			for (int i=1; i < queue.size(); i++){
				if (queue[idx].second > queue[i].second) idx = i;
			}
			glm::ivec2 current = queue[idx].first;
			queue.erase(queue.begin() + idx);

			// Gestion de l'arrivee
			if (current == destination) {
				new_trajet.push_back(destination); 
				break;
			}

			// Generation du trajet
			for (glm::ivec2 next : edges[current]){
				int nClout = clout[current];
				if ( (clout.find(next) != clout.end()) || (nClout < clout[next]) ) {
					clout[next] = nClout;
					glm::ivec2 nCXY = current;
					int prio = nClout + abs(destination[0] - nCXY[0]) + abs(destination[1] - nCXY[1]);
					std::pair tuple (next, prio);
					queue.push_back(tuple);
					new_trajet.push_back(current);
					done++;
				}
			}
		}
		return new_trajet;

	} else return new_trajet;
}

void Robot::setTrajet(std::vector<glm::ivec2> trj) {
	case_of_t = 0;
	trajet.clear();
	trajet = trj;
}