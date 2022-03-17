
#include <Multiplayer/Server.hpp>

using namespace std;

Server::ServerClient::ServerClient (Server * server, Socket && socket)
	: m_server {server}, m_socket {move(socket)}, m_active {false}
{
	cerr << "Nouveau client !" << endl;
}

void Server::ServerClient::start() {
	if (m_active) return;

	// Pointeur intelligent pour assurer la survie de l'objet.
	ServerClientPtr self = shared_from_this();

	// Lecture asynchrone.
	async_read_until(m_socket, m_buffer, '\n', [this, self] (const error_code & ec, size_t n) {
		UNUSED(n);
		// Erreur ?
		if (!ec) {
			istream is {&m_buffer};
			string alias;
			getline(is, alias, ' ');
			alias = regex_replace(alias, regex("\\s"), "");

			if (m_server->find(alias) == nullptr) {
				cerr << "Nouveau client avec l'username : " << alias << endl;
				self->m_alias = alias;
				self->write("#alias " + alias);
				m_server->process_list(self);
				m_server->broadcast("#connected " + alias, self);
				self->m_active = true;
				self->read();
			}
			else
				self->write(Server::INVALID_ALIAS);
		}
		else {
			cerr << "Bonjour, au revoir !" << endl;
			m_server->m_clients.remove(self);
		}
	});
}

void Server::ServerClient::stop() { m_active = false; }

void Server::ServerClient::rename(const string & alias) {
	m_alias = alias;
	write("#alias " + alias);
}

void Server::ServerClient::read() {

	// Pointeur intelligent pour assurer la survie de l'objet.
	ServerClientPtr self = shared_from_this();

	// Lecture asynchrone.
	async_read_until(m_socket, m_buffer, '\n', [this, self] (const error_code & ec, size_t n) {
		UNUSED(n);
		// Erreur ?
		if (!ec) {
			istream is {&m_buffer};
			string message;

			// Traiter tous les messages disponibles.
			while (getline(is, message))
				m_server->process(self, message);
			
			// Si le client est toujours actif, lire à nouveau.
			if (m_active) read();
		}
		else {
			cerr << "Déconnexion !" << endl;
			m_server->broadcast("#disconnected " + self->alias(), self);
			m_server->m_clients.remove(self);
			m_server->process_quit(self, string {});
		}
	});
}

void Server::ServerClient::write(const string & message) {

	// Ajout du caractère "fin de ligne".
	string m = message + '\n';

	// Écriture asynchrone.
	async_write(
		m_socket,
		asio::buffer (m.data (), m.length ()),
		[this] (const error_code & ec, size_t n) { UNUSED(ec); UNUSED(n); }
	);
}
