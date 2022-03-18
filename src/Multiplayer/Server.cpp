
#include <Multiplayer/Server.hpp>

using namespace std;

Server::Server(unsigned short port) : m_context {}, m_clients {},
	m_acceptor {m_context, asio::ip::tcp::endpoint {asio::ip::tcp::v4(), port}}
{
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

void Server::process_private(ServerClientPtr client, const string & data) {
	int delimiterPosition = data.find(' ');
	string recipientName = data.substr(0, delimiterPosition);
	string message = data.substr(delimiterPosition + 1, data.length() - 1);

	ServerClientPtr recipientClient = find(recipientName);
	if(recipientClient != nullptr)
		recipientClient->write("#private " + client->alias() + " " + message);
}

void Server::process_list(ServerClientPtr client, const string & data) {
	UNUSED(data);
	string m = "#list ";
	for (ServerClientPtr client: m_clients) {
		m += client->alias() + " ";
		cerr << "-> [" << client->alias() << "]" << endl;
	}
	m.pop_back();
	client->write(m);
}

void Server::process_amogus(ServerClientPtr client, const string & data) {
	UNUSED(client);
	UNUSED(data);
	broadcast("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀⠀⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣸⣿⡏⠀⠀⠀⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⡄⠀\n"
			"⠀⠀⣀⣤⣴⣶⣶⣿⡟⠀⠀⠀⢸⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀\n"
			"⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿⠀\n"
			"⠀⢸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀\n"
			"⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇⠀⠀\n"
			"⠀⣿⣿⠁⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⠀⠀\n"
			"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n"
			"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n"
			"⠀⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀\n"
			"⠀⠸⣿⣧⡀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀\n"
			"⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⣽⣿⡏⠁⠀⠀⢸⣿⡇⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁⠀⠈⠻⣿⣿⣿⣿⡿⠏⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
}

void Server::process_message(ServerClientPtr client, const string & data) {
	string m = "<b>" + client->alias() + "</b> : " + data;
	broadcast (m);
}

void Server::process_quit(ServerClientPtr client, const string & data) {
	UNUSED(data);
	client->stop();
}

void Server::broadcast(const string & message, ServerClientPtr emitter) {
	for (ServerClientPtr client: this->m_clients)
		if (client != emitter)
			client->write(message);
}

const map<string, Server::Processor> Server::PROCESSORS {
	{"/join", &Server::process_join},
	{"/quit", &Server::process_quit},
	{"/list", &Server::process_list},
	{"/private", &Server::process_private},
	{"/ඞ", &Server::process_amogus},
};

const string Server::INVALID_ALIAS		{"#error invalid_alias"};
const string Server::INVALID_COMMAND	{"#error invalid_command"};
const string Server::INVALID_RECIPIENT	{"#error invalid_recipient"};
const string Server::MISSING_ARGUMENT	{"#error missing_argument"};
