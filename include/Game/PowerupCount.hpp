#pragma once

#include <Game/ObjectPerk.hpp>

class PowerupCount: public ObjectPerk
{
public:
	PowerupCount(Map* map, glm::ivec2 coord);
	virtual ~PowerupCount() {}
};
