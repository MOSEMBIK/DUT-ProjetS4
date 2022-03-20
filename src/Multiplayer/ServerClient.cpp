
#include <Multiplayer/Server.hpp>

using namespace std;

Server::ServerClient::ServerClient (Server * server, Socket && socket)
	: m_server {server}, m_socket {move(socket)}, m_active {false}, m_id {m_current_id++}
{
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

			// Si l'alias est déjà pris, on rajoute un chiffre.
			if (m_server->find(alias) != nullptr) {
				int i = 2;
				while (m_server->find(alias + "_" + to_string(i)) != nullptr)
					i++;
				alias += "_" + to_string(i);
			}

			self->m_alias = alias;
			self->m_active = true;
			self->write("#connected " + to_string(self->m_id));
			self->read();
		}
		else {
			m_server->broadcast("#disconnected " + to_string(self->m_id));
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
			m_server->broadcast("#disconnected " + self->alias(), self);
			m_server->m_clients.remove(self);
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
