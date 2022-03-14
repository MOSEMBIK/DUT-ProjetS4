#include <Game/Robot.hpp>
#include <Game/Map.hpp>

Robot::Robot(Map* map) : Player(map) {
	this->mapSize = map->getSize() - 1;
	this->x = rand() % mapSize;
	this->z = rand() % mapSize;
}


void Robot::update(float deltaTime) {
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

// Mannathan A* algorithm
std::vector<glm::ivec2> Robot::genTrajet(glm::ivec2 destination) {

	std::vector<glm::ivec2> new_trajet;
	new_trajet.push_back(trajet[case_of_t]);

	int done = 0;

	if (map->isReachable(destination)){
		
		std::map<glm::ivec2, std::vector<glm::ivec2>, cmpVec> &edges = *&(map->edges_map);

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
			for (int i=1; i < int(queue.size()); i++){
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
		case_of_t = 0;
		return new_trajet;

	} else return new_trajet;
}