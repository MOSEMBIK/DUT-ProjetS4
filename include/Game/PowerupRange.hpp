#pragma once

#include <Game/ObjectPerk.hpp>
#include <Game/Map.hpp>

class PowerupRange: public ObjectPerk
{
public:
	PowerupRange(Map* map);
	virtual ~PowerupRange() {}
	virtual void onPickUp(Player*);

};
