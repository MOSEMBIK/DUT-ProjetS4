#pragma once

#include <Game/Actor.hpp>
#include <Game/Player.hpp>

class ObjectPerk: public Actor
{
private:
	float lifetime;

public:
	ObjectPerk();
	virtual void onPickUp(Player*) = 0;

};
