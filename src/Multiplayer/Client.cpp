
#include <Multiplayer/Client.hpp>
#include <Game/Map.hpp>
#include <Game/Game.hpp>

using namespace std;

Client::Client(Game* game, const string & host, unsigned short port) : io_context(), m_socket(io_context), m_game(game)
{
	// Connexion au serveur
	m_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(host), port));

	// Lancement du thread
	m_thread = thread([this]() { io_context.run(); });

	async_read_until(m_socket, m_buffer, '\n', [this] (const error_code & ec, size_t n) {
		UNUSED(n);
		if (!ec) handle_read();	
	});
}

void Client::handle_read() {
	error_code ec;
	read_until(m_socket, m_buffer, '\n', ec);

	if (!ec) {
		istream is {&m_buffer};
		string message;
		getline(is, message);
		if (message != "")
			process(message);
		
		handle_read();
	}
}

void Client::stop() {
	// Fermeture du socket.
	m_socket.close();
	// Arrêt du thread.
	m_thread.~thread();
}

// Envoi d'un message à travers le socket.
void Client::write(const string & message) {
	// Ajout du caractère "fin de ligne".
	string m = message + '\n';

	async_write(
		m_socket,
		asio::buffer (m.data (), m.length ()),
		[this] (const error_code & ec, size_t n) { UNUSED(ec); UNUSED(n); }
	);
}

void Client::message (const string & message) {
	// Envoi du message.
	write(message);
}

void Client::error (const string & id) {
	// Envoi de la commande.
	write("#error " + id);
}

void Client::process(const string & message) {
	istringstream iss (message);
	string command;
	if (iss >> command) {
		if (command[0] == '#') {
			iss >> ws;
			string data {istreambuf_iterator<char> {iss}, istreambuf_iterator<char> {}};

			auto search = PROCESSORS.find(command);
			if(search != PROCESSORS.end()) {
				Client::Processor proc = search->second;
				(this->*proc)(data);
			}
		}
	}
}

// Commande "#connected"
void Client::process_connected(const string & message) {
	m_game->m_connected = message;
}

// Commande "#loadMap"
void Client::process_loadMap(const string & message) {
	m_game->m_mapInfo = message;
}

// Commande "#updatePosRot"
void Client::process_updatePosRot(const string & message) {
	m_game->m_updatePosRot = message;
}

// Commande "#playerslist"
void Client::process_playersList(const string & message) {
	m_game->m_playersList = message;
}

// Commande "#bomb"
void Client::process_bomb(const string & message) {
	m_game->m_newBombs += message + ";";
}

// Commande "#restart"
void Client::process_restart(const string & message) {
	UNUSED(message);
	if (m_game->m_server == nullptr) {
		m_game->m_connected = to_string(m_game->m_playerId);
		m_game->setState(GameState::MULTI_JOIN_SERVER);
	}
}
