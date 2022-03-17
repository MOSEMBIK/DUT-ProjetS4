#pragma once

#include <Multiplayer/Server.hpp>

class Client {
  private:
	// Signature d'un processeur.
	typedef void (Client::*Processor) (const std::string &);
	// Processeurs.
	static const std::map<std::string, Processor> PROCESSORS;

  private:
	Socket m_socket;

  private:
	// Traitement d'un message.
	void process (const std::string & message);
	// Gestion des erreurs.
	void process_error (const std::string & message);
	// Reste des processors
	void process_alias (const std::string & message);
	void process_connected (const std::string & message);
	void process_disconnected (const std::string & message);
	void process_renamed (const std::string & message);
	void process_list (const std::string & message);
	void process_private (const std::string & message);

  public:
	// constructeur : nom du serveur, port et, éventuellement, objet parent.
	Client (const std::string & host, unsigned short port = 42069);
	~Client ();

	// Envoi d'un message.
	void write (const std::string &);

	// Connexion / déconnexion.
	void connected (const std::string & host, unsigned short port = 42069);
	void disconnected ();
	// Message.
	void message (const std::string & message);
	// Error.
	void error (const std::string & id);
	// Reste des signaux
	void alias (const std::string &);
	void user_connected (const std::string &);
	void user_disconnected (const std::string &);
	void user_renamed (const std::string &, const std::string &);
	void user_list (const std::string &);
	void user_private (const std::string &, const std::string &);
};

// Processeurs.
const std::map<std::string, Client::Processor> Client::PROCESSORS = {
	{"#error", &Client::process_error},
	{"#alias", &Client::process_alias},
	{"#connected", &Client::process_connected},
	{"#disconnected", &Client::process_disconnected},
	{"#renamed", &Client::process_renamed},
	{"#list", &Client::process_list},
	{"#private", &Client::process_private}
};

