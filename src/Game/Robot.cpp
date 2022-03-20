#include <Game/Robot.hpp>
#include <Game/Map.hpp>

using std::cerr;
using std::endl;

Robot::Robot(Map* map, glm::ivec2 pos) : Player(map, pos) {
	case_of_t = 0;
	trajet.clear();
	trajet.push_back(glm::ivec2(x, z));
}

void Robot::update(float deltaTime) {
	glm::vec3 pos = m_transform.getPosition();
	if (pos.x > x) pos.x += 0.9f;
	if (pos.z > z) pos.z += 0.9f;


	cerr << "Updating Robot..." << endl;
	Transform& transform = getTransform();
	
	// glm::ivec2 sBomb = shouldBomb();
	// if (sBomb[0] != -1 && sBomb[1] != -1) {
	// 	this->setBomb(sBomb);
	// 	setTrajet(genTrajetMann(choseDestination()));
	// }

	if (trajet.size()<=1 || (trajet[case_of_t].x == trajet.back().x && trajet[case_of_t].y == trajet.back().y)) {
		cerr << "Trajet fini" << endl;
		setTrajet(genTrajetMann(choseDestination(9)));
	}

	if (case_of_t + 1 < trajet.size() && (int(pos.x) == x && int(pos.z) == z)) {
		case_of_t++;
	}
	x = trajet[case_of_t][0];
	z = trajet[case_of_t][1];

	cerr << pos.x << " - " << pos.z << endl;
	cerr << x << " - " << z << endl;
	cerr << "Case of T : " << case_of_t << endl;
	cerr << "trajet"<< endl;
	cerr << "{ "<< endl;
	for (auto var : trajet){
		cerr << "( " << var[0] << ", " << var[1] << " ), "<< endl;
	}
	cerr << " }" << endl;
	Player::update(deltaTime);
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
bool Robot::isPossible(glm::ivec2 coord, glm::ivec2 aPos, std::list<glm::ivec2> *checked){
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;
	glm::ivec2 pos;
	if (aPos[0] == -1 && aPos[1] == -1) pos = trajet[case_of_t];
	else  pos = aPos;

	checked->push_back(pos);

	for (glm::ivec2 next : edges[pos]) {
		if (find(checked->begin(), checked->end(), next) == checked->end()){
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
glm::ivec2 Robot::choseDestination(int mode){
	std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;
	std::map<glm::ivec2, float, cmpVec> dangerMap = map->danger_map;
	std::list<std::pair<glm::ivec2, int>>  players = map->getPlayersMap();
	cerr << "Danger Map"<< endl;
	cerr << "{ "<< endl;
	for (auto var : dangerMap){
		cerr << "( " << var.first[0] << ", " << var.first[1] << " )[ " << var.second << " ], " << endl;
	}
	cerr << " }" << endl;
	glm::ivec2 pos = trajet[case_of_t];

	// Explosion safe location
	if (mode == 0) { 						
		glm::ivec2 nearstSafe = pos;
		std::list<glm::ivec2> checked;
		
		for (glm::ivec2 loc : edges[nearstSafe]){
			if (find(checked.begin(), checked.end(), loc) == checked.end()) {
				checked.push_back(loc);
				if (dangerMap[loc] > dangerMap[nearstSafe]) nearstSafe = loc;
			}
		}

		if (dangerMap[nearstSafe] != 1) {
			nearstSafe = edges[pos][0];
			for (glm::ivec2 loc : edges[pos]){
				if (dangerMap[nearstSafe] != 1) {
					nearstSafe = loc;
					for (glm::ivec2 loc2 : edges[nearstSafe]){
						if (find(checked.begin(), checked.end(), loc2) == checked.end()) {
							checked.push_back(loc2);
							if (dangerMap[loc2] > dangerMap[nearstSafe]) nearstSafe = loc;
						}
					}
				}
			}
		}
		
		if (isPossible(nearstSafe)) return nearstSafe;

	// Nearst Player location
	} else if (mode == 1) { 
		cerr << "( " << pos.x << ", " << pos.y << " )" << endl;
		std::list<glm::ivec2> playersL;
		for (std::pair<glm::ivec2, int> pl : players){
			if (pl.second != id){
				playersL.push_back(pl.first);
				cerr << "( " << pl.first.x << ", " << pl.first.y << " )" << endl;
			}
			cerr << "C'est moi !!" << endl;
		}
		
		glm::ivec2 nearstPlayer = playersL.front();
		for (glm::ivec2 ply : playersL) if (sqrt(abs(ply[0] - pos[0]) + abs(ply[1] - pos[1])) < sqrt(abs(nearstPlayer[0] - pos[0]) + abs(nearstPlayer[1] - pos[1]))) nearstPlayer = ply;
		if (isPossible(nearstPlayer)) return nearstPlayer;

	// Debug mode
	}	else if (mode == 9) { 
		cerr << " CHOSE DEST : DEBUG MODE " << endl;
		
		x = rand() % mapSize;
		z = rand() % mapSize;
		glm::ivec2 dest (x, z);
		cerr << "Choose : ( " << x << ", " << z << " )" << endl;

		cerr << " IsPossible ? " << endl;
		bool possi = isPossible(dest);
		if (possi) {
			cerr << "Destination possible " << endl;
			return dest;
		} else {
			cerr << "Destination not possible" << endl;
			x = pos.x;
			z = pos.y;
		}
		
	}	
	return pos;
}

// Bombs
/**
 * @brief Return were the Robot can place a Bomb (return (-1, -1) if cannot)
 * 
 * @return Placement location
 */
glm::ivec2 Robot::shouldBomb(){
	cerr << "Should bomb function" << endl;
	if (case_of_t >= int(trajet.size())-1) {
		if (trajet.size() <= 1){
			cerr << "Set Trajet" << endl;
			setTrajet(genTrajetMann(choseDestination(1)));

			cerr << "Return (-1, -1)" << endl;
			return glm::ivec2 (-1, -1);
		}
		cerr << "Return Bomb" << endl;
		return trajet[case_of_t];
	}
	cerr << "Return (-1, -1)" << endl;
	return glm::ivec2 (-1, -1);
}

///--- DEPLACEMENTS
/**
 * @brief Mannathan A* Algorithm
 * @param destination Destination location
 * @return Vector of coords that represent the best way found to reach destination
*/
std::vector<glm::ivec2> Robot::genTrajetMann(glm::ivec2 destination) {
	cerr << "Generate Trajet A* function" << endl;

	std::vector<glm::ivec2> new_trajet;
	new_trajet.push_back(trajet[case_of_t]);
	int done = 0;

	if (map->isReachable(destination) && isPossible(destination)){
		std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> edges = map->edges_map;
		std::vector<std::pair<glm::ivec2, int>> queue;
		queue.push_back(std::pair<glm::ivec2, int> (new_trajet.back(), 0));
		cerr << "Start : ( " << new_trajet[0].x << ", " << new_trajet[0].y << " )" << endl;
		cerr << "Arrivee : ( " << destination.x << ", " << destination.y << " )" << endl;

		std::map<glm::ivec2, int, cmpVec> clout;
		clout[new_trajet[done]] = 0;

		// Génération plus court chemin
		while (!queue.empty()) {
			cerr << queue.size() << endl;
			cerr << "Queue"<< endl;
			cerr << "{ "<< endl;
			for (auto var : queue){
				cerr << "( " << var.first[0] << ", " << var.first[1] << " ){ " << var.second << " }, "<< endl;
			}
			cerr << " }" << endl;

			// Recuperation de la case optimale
			int idx = 0;
			for (int i=1; i < int(queue.size()); i++){
				if (queue[idx].second > queue[i].second) idx = i;
			}
			cerr << idx << endl;
			glm::ivec2 current = queue[idx].first;
			queue.erase(queue.begin() + idx);

			// Gestion de l'arrivee
			if (current.x == destination.x && current.y == destination.y) {
				cerr << "Arrivé !" << endl;
				new_trajet.push_back(destination); 
				break;
			}

			// Generation du trajet
			for (glm::ivec2 next : edges[current]){
				int nClout = clout[current];
				if ( (clout.find(next) == clout.end()) || (nClout < clout[next]) ) {
					cerr << "If " << endl;
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

		std::vector<glm::ivec2> path;
		int inPath = 0;
		path.push_back(new_trajet[0]);
		for (int i=0; i < new_trajet.size(); i++){
			std::vector<glm::ivec2> nears = edges[path[inPath]];
			if (find(nears.begin(), nears.end(), new_trajet[i]) != nears.end()){
				path.push_back(new_trajet[i]);
				inPath++;
			}
		}

		cerr << " }" << endl;

		cerr << "trajet"<< endl;
		cerr << "{ "<< endl;
		for (auto var : path){
			cerr << "( " << var[0] << ", " << var[1] << " ), "<< endl;
		}
		cerr << " }" << endl;

		return path;

	} else {
		return new_trajet;
	}
}

void Robot::setTrajet(std::vector<glm::ivec2> trj) {
	trajet.clear();
	trajet = trj;
	case_of_t = 0;
}
