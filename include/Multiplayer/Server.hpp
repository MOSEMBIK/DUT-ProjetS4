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
	class ServerClient : public std::enable_shared_from_this<ServerClient> {
	  private:
		Server * m_server;
		Socket m_socket;
		asio::streambuf m_buffer;
		std::string m_alias;
		bool m_active;
		int m_id;
		static int m_current_id;
		
	  public:
		ServerClient (Server *, Socket &&);
		void start ();
		void stop ();
		inline std::string alias () const { return m_alias; }
		inline int getId() const { return m_id; }
		void rename (const std::string &);
		void read ();
		void write (const std::string &);
	};

	// Pointeur intelligent.
	typedef std::shared_ptr<ServerClient> ServerClientPtr;
	// Signature d'un processeur.
	typedef void (Server::*Processor) (ServerClientPtr, const std::string &);
	// Processeurs.
	static const std::map<std::string, Processor> PROCESSORS;

  private:
	std::thread m_thread;
	asio::io_context m_context;
	std::list<ServerClientPtr> m_clients;
	asio::ip::tcp::acceptor m_acceptor;

  private:
	// Connexions entrantes.
	void accept ();
	// Recherche par alias.
	ServerClientPtr find (const std::string & alias);

	// Traitement des commandes
	void process (ServerClientPtr, const std::string &);
	void process_join (ServerClientPtr client, const std::string & = "");
	void process_quit (ServerClientPtr client, const std::string & = "");
	void process_list (ServerClientPtr client, const std::string & = "");
	void process_private (ServerClientPtr client, const std::string & = "");
	void process_amogus (ServerClientPtr client, const std::string & = "");

	void process_message (ServerClientPtr, const std::string &);

	void remove (ServerClientPtr);

  private:
	Map m_map;

  public:
	Server (unsigned short port = 42069);
	~Server ();
	void start (); // Démarrage.

	inline void setMap (Map map) { m_map = map; }
	inline Map& getMap () { return m_map; }
	inline std::vector<int> getClientsIds () { std::vector<int> v; for (auto c: m_clients) v.push_back(c->getId()); return v; }

	// Diffusion d'un message.
	void broadcast (const std::string & message, ServerClientPtr emitter = nullptr);
	void sendList (const std::string & message);

  public:
	static const std::string INVALID_ALIAS;
	static const std::string INVALID_COMMAND;
	static const std::string INVALID_RECIPIENT;
	static const std::string MISSING_ARGUMENT;
};

