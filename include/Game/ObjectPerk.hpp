#pragma once

#include <Game/Actor.hpp>

class ObjectPerk: public Actor
{
public:
	enum Type { None, Count, Range, Speed };

	ObjectPerk(Map* map, glm::ivec2 coord, Type type);
	virtual ~ObjectPerk() {}

	inline ObjectPerk::Type getType() {return this->type;};

private:	
	float lifetime = 10;
	Type type;

	
};
