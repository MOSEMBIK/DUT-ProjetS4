#pragma once

#include <Game/ObjectPerk.hpp>
#include <Game/Map.hpp>

class PowerupCount: public ObjectPerk
{
public:
	PowerupCount(Map* map);
	virtual void onPickUp(Player*);

};
