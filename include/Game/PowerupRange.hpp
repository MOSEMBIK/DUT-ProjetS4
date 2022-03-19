#pragma once

#include <Game/ObjectPerk.hpp>
#include <Game/Map.hpp>

class PowerupRange: public ObjectPerk
{
public:
	PowerupRange(Map* map, glm::ivec2 coord);
	virtual ~PowerupRange() {}

};
