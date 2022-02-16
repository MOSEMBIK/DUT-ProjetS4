#pragma once

#include <Game/ObjectPerk.hpp>

class PowerupCount: public ObjectPerk
{
public:
	PowerupCount();
	virtual void onPickUp(Player*);

};
