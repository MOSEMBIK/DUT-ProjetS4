#pragma once

#include <utils.h>

#include <Game/Map.hpp>

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <asio.hpp>
#include <regex>

struct ServerInfo {
	std::string		m_ip = "127.0.0.1";
	std::string		m_serverOwner = "PielleBoule";
	std::string		m_players = "1:4";
	unsigned int	m_ping = 999;
};

typedef asio::ip::tcp::socket Socket;

class Server {
  private:
	// Client vu du serveur (pointeurs intelligents).
	class Client : public std::enable_shared_from_this<Client> {
	  private:
		Server * m_server;
		Socket m_socket;
		asio::streambuf m_buffer;
		std::string m_alias;
		bool m_active;
		
	  public:
		Client (Server *, Socket &&);
		void start ();
		void stop ();
		inline std::string alias () const { return m_alias; }
		void rename (const std::string &);
		void read ();
		void write (const std::string &);
	};

	// Pointeur intelligent.
	typedef std::shared_ptr<Client> ClientPtr;
	// Signature d'un processeur.
	typedef void (Server::*Processor) (ClientPtr, const std::string &);
	// Processeurs.
	static const std::map<std::string, Processor> PROCESSORS;

  private:
	asio::io_context m_context;
	std::list<ClientPtr> m_clients;
	asio::ip::tcp::acceptor m_acceptor;

  private:
	// Connexions entrantes.
	void accept ();
	// Recherche par alias.
	ClientPtr find (const std::string & alias);
	// Traitement d'une commande.
	void process (ClientPtr, const std::string &);
	// Traitement de la commande quit.
	void process_quit (ClientPtr client, const std::string &);
	// Traitement de la commande list.
	void process_list (ClientPtr client, const std::string & = "");
	// Traitement de la commande private.
	void process_private (ClientPtr client, const std::string & = "");
	// Traitement de la commande amogus.
	void process_amogus (ClientPtr client, const std::string & = "");
	// Processeurs.
	void process_message (ClientPtr, const std::string &);
	// Diffusion d'un message.
	void broadcast (const std::string & message, ClientPtr emitter = nullptr);
	// Suppression d'un client.
	void remove (ClientPtr);

  private:
	Map m_map;

  public:
	Server (unsigned short port = 42069);
	void start (); // Démarrage.

	inline void setMap (Map map) { m_map = map; }
	inline Map& getMap () { return m_map; }

  public:
	static const std::string INVALID_ALIAS;
	static const std::string INVALID_COMMAND;
	static const std::string INVALID_RECIPIENT;
	static const std::string MISSING_ARGUMENT;
};

