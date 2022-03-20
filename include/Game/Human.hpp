#pragma once

#include <Game/Player.hpp>
#include <Game/Map.hpp>
#include <Multiplayer/Client.hpp>

class Human: public Player
{
private:
	unsigned int keyPressed = 0;
	bool spacePressed = false;
	Client* m_client;

public:
	Human(Map* map, glm::ivec2 pos);
	Human(Map* map, std::string& data);
  	virtual ~Human() {}

	void update(float deltaTime);

};
