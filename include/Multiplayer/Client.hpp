#pragma once

#include <Multiplayer/Server.hpp>

class Game;

class Client {
  private:
	asio::io_context io_context;
	Socket m_socket;
	std::thread m_thread;
	asio::streambuf m_buffer;
	Game* m_game;

  private:
	// Traitement d'un message.
	void process (const std::string & message);
	// Gestion des erreurs.
	void process_error (const std::string & message);
	// Reste des processors
	void process_connected (const std::string & message);
	void process_loadMap (const std::string & message);
	void process_updatePosRot (const std::string & message);
	void process_playersList (const std::string & message);
	void process_bomb (const std::string & message);
	void process_restart (const std::string & message);

  public:
	// constructeur : nom du serveur, port et, éventuellement, objet parent.
	Client (Game* game, const std::string & host, unsigned short port = 42069);
	void stop ();

	// Envoi d'un message.
	void write (const std::string &);
	// Lecture d'un message.
	void handle_read ();

	// Message.
	void message (const std::string & message);
	// Error.
	void error (const std::string & id);

  private:
	// Signature d'un processeur.
	typedef void (Client::*Processor) (const std::string &);
	// Processeurs.
	const std::map<std::string, Processor> PROCESSORS  = {
		{"#connected", &Client::process_connected},
		{"#loadMap", &Client::process_loadMap},
		{"#updatePosRot", &Client::process_updatePosRot},
		{"#playersList", &Client::process_playersList},
		{"#bomb", &Client::process_bomb},
		{"#restart", &Client::process_restart}
	};
};
