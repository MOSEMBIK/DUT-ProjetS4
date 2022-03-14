#pragma once

#include <string>

struct ServerInfo {
	std::string		m_ip = "127.0.0.1";
	std::string		m_serverOwner = "PielleBoule";
	std::string		m_players = "1/4";
	unsigned int	m_ping = 999;
};

class Server {
};
