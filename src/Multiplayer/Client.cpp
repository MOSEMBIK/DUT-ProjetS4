
#include <Multiplayer/Client.hpp>

using namespace std;

Client::Client(const string & host, unsigned short port) : io_context(), m_socket(io_context)
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

Client::~Client() {
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

void Client::alias (const string & alias) {
	// Envoi de la commande.
	write("#alias " + alias);
}

void Client::user_connected (const string & username) {
	// Envoi de la commande.
	write("#user_connected " + username);
}

void Client::user_disconnected (const string & username) {
	// Envoi de la commande.
	write("#user_disconnected " + username);
}

void Client::user_renamed (const string & old_username, const string & new_username) {
	// Envoi de la commande.
	write("#user_renamed " + old_username + " " + new_username);
}

void Client::user_list (const string & message) {
	// Envoi de la commande.
	write("#user_list " + message);
}

void Client::process(const string & message) {
	istringstream iss (message);
	string command;
	if (iss >> command)
		if (command[0] == '#') {
			iss >> ws;
			string data {istreambuf_iterator<char> {iss}, istreambuf_iterator<char> {}};

			auto search = PROCESSORS.find(command);
			if(search != PROCESSORS.end()) {
				Client::Processor proc = search->second;
				(this->*proc)(data);
			}
		}
		else {
			cerr << message << endl;
		}
}


// Commande "#alias"
void Client::process_alias(const string & message)
{
	cerr << "Client::process_alias: " << message << endl;
}

// Commande "#connected"
void Client::process_connected(const string & message)
{
	cerr << "Client::process_connected: " << message << endl;
}

// Commande "#disconnected"
void Client::process_disconnected(const string & message)
{
	cerr << "Client::process_disconnected: " << message << endl;
}

// Commande "#renamed"
void Client::process_renamed(const string & message)
{
	cerr << "Client::process_renamed: " << message << endl;
}

// Commande "#list"
void Client::process_list(const string & message)
{
	cerr << "Client::process_list: " << message << endl;
}

// Commande "#private"
void Client::process_private(const string & message)
{
	cerr << "Client::process_private: " << message << endl;
}

// Commande "#error"
void Client::process_error (const string & message)
{
	cerr << "Client::process_error: " << message << endl;
}
