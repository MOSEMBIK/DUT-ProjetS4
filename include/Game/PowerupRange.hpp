#pragma once

#include <Game/ObjectPerk.hpp>

class PowerupRange: public ObjectPerk
{
public:
	PowerupRange();
	virtual void onPickUp(Player*);

};
