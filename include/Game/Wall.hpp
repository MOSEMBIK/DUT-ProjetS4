#pragma once

#include <Game/Actor.hpp>

class Wall: public Actor
{
public:
	enum class Type { Metal, Stone, Wood };
	Wall(Map* map, Type type = Type::Metal);
	virtual ~Wall() {}

	std::string getData() const; // Fonction qui retourne les données du mur sous forme de string

	inline Type getType() { return type; }
	inline int getHealth() { return health; }
	void removeHealth();

private:
	Type type;
	int health;

	void onDestroy();


};
