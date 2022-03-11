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

std::vector<glm::ivec2> Robot::move(glm::ivec2 destination) {

	std::vector<glm::ivec2> new_trajet;
	new_trajet.push_back(trajet[case_of_t]);

	int done = 0;

	if (map->isReachable(destination)){
		/*
		if trajet[done].type != 'road':
			trajet.append(plateau.nearRoads(trajet[done])[0])
			done += 1
		*/
		std::vector<std::pair<int, glm::ivec2>> queue;
		queue[0] = (std::pair<int, glm::ivec2> (0, trajet.back()));

		clout = {}
		clout[trajet[done]] = 0

		std::unordered_map<glm::ivec2, std::vector<glm::ivec2>> &edges = *&(map->edges_map);
		std::vector<char> queue;
		// Génération plus court chemin
		while (queue.empty()) {
			// Recuperation de la case optimale
			/*
			idx = 0
			for i in range(1, len(queue)):
				if queue[idx][0] > queue[i][0]:
					idx = i
			current = queue[idx][1]
			queue.remove(queue[idx])
			*/
			glm::ivec2 current (0, 0);
			// Gestion de l'arrivee
			if (current == destination) {
				trajet.push_back(destination); 
				break;
			}
			/*
			// Generation du trajet
			for next in edges[current]:
				nClout = clout[current]
				if next not in clout or nClout < clout[next]:
					clout[next] = nClout
					nCXY = current.getCoords()
					prio = nClout + abs(destination.getCoords()[0] - nCXY[0]) + abs(destination.getCoords()[1] - nCXY[1])
					queue.append((prio, next))
					trajet.append(current)
					done += 1
		}

		pathing: list[Case] = [trajet[len(trajet) - 1]]
		inPath = 0
		for i in range(len(trajet) - 2, -1, -1):
			if trajet[i] in plateau.nearLieu(pathing[inPath]) or trajet[i] in plateau.nearRoads(pathing[inPath]):
				pathing.append(trajet[i])
				inPath += 1
		pathing.reverse()

		return pathing
		*/
	} else return trajet;
}