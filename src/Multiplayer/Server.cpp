
#include <Multiplayer/Server.hpp>

using namespace std;

int Server::ServerClient::m_current_id = 2022000;

Server::Server(unsigned short port) : m_context {}, m_clients {},
	m_acceptor {m_context, asio::ip::tcp::endpoint {asio::ip::tcp::v4(), port}}
{
}

void Server::stop() {
	for (auto client : m_clients)
		client->stop();
	// Fermeture du socket.
	m_acceptor.close();
	// Arrêt du thread.
	//m_thread.~thread();
}

void Server::start() {
	// Acceptation des connexions entrantes.
	accept();

	// Démarrage du contexte sur un nouveau thread
	m_thread = thread([this]() {
		m_context.run();
	});
}

Server::ServerClientPtr Server::find(const string & alias) {
	for (ServerClientPtr client: this->m_clients)
		if(client->alias() == alias)
			return client;
	return nullptr;
}

void Server::accept() {
	m_acceptor.async_accept ([this](const error_code & ec, Socket && socket) {
		// Erreur ?
		if (!ec) {
			m_clients.emplace_back(make_shared<ServerClient>(this, move (socket)));
			m_clients.back()->start();
		}
		accept();
	});
}

void Server::process(ServerClientPtr client, const string & message) {
	// Lecture d'une éventuelle commande.
	istringstream iss (message);
	string command;
	if (iss >> command) {
		// Commande ?
		if (command[0] == '/') {
			// Consommation des caractères blancs.
			iss >> ws;
			// Reste du message.
			string data {istreambuf_iterator<char> {iss}, istreambuf_iterator<char> {}};

			// Recherche du processeur correspondant.
			// - S'il existe, l'appeler ;
			// - Sinon, "#invalid_command" !
			auto search = PROCESSORS.find(command);
			if(search != PROCESSORS.end()) {
				Server::Processor proc = search->second;
				(this->*proc)(client, data);
			}
			else
				client->write(Server::INVALID_COMMAND);
		}
	else
		process_message (client, message);
	}
}

void Server::process_join(ServerClientPtr client, const string & data) {
	UNUSED(data);
	// Récupération des informations de la map.
	string map = m_map.getData();

	// Envoie des informations de la map.
	client->write(map);
}

void Server::process_move(ServerClientPtr client, const string & data) {
	int id = client->getId();
	int x = stoi(data.substr(0, data.find(",")));
	int z = stoi(data.substr(data.find(",") + 1));
	m_map.movePlayer(id, x, z);
}

void Server::process_bomb(ServerClientPtr client, const string & data) {
	int id = client->getId();
	string player = m_map.getPlayerData(id);
	if (player != "") {
		int x = stoi(data.substr(0, data.find(",")));
		int z = stoi(data.substr(data.find(",") + 1));

		vector<string> playerData;
		for (int i = 0; i < (int)player.size(); i++) {
			if (player[i] == ',') {
				playerData.push_back(player.substr(0, i));
				player = player.substr(i + 1, player.size() - i - 1);
				i = 0;
			}
		} playerData.push_back(player);

		m_map.addBomb(
			new Bomb(
				&m_map,
				glm::vec3(stof(playerData[6]), stof(playerData[7]), stof(playerData[8])),
				stoi(playerData[10])
			),
			glm::ivec2(x,z)
		);
		broadcast("#bomb " + to_string(x) + "," + to_string(z) + "," + playerData[6] + "," + playerData[7] + "," + playerData[8] + "," + playerData[10]);
	}
}


void Server::process_message(ServerClientPtr client, const string & data) {
	string m = client->alias() + " : " + data;
	broadcast (m);
}

void Server::broadcast(const string & message, ServerClientPtr emitter) {
	for (ServerClientPtr client: this->m_clients)
		if (client != emitter)
			client->write(message);
}

void Server::sendList(const string & message) {
	string list = "#playersList " + message + ";";
	for (ServerClientPtr client: this->m_clients)
		list += client->alias() + ",";
	broadcast(list);
}

const map<string, Server::Processor> Server::PROCESSORS {
	{"/join", &Server::process_join},
	{"/move", &Server::process_move},
	{"/bomb", &Server::process_bomb},
};

const string Server::INVALID_ALIAS		{"#error invalid_alias"};
const string Server::INVALID_COMMAND	{"#error invalid_command"};
const string Server::INVALID_RECIPIENT	{"#error invalid_recipient"};
const string Server::MISSING_ARGUMENT	{"#error missing_argument"};
