#pragma once
#include <cstdint>
class Entity
{
private:
	uint32_t id;
	uint32_t hasComponentBits;

public:

	//whether or not an entity has x component
	static const uint32_t FLAG_TRANSFORM_COMPONENT = 0x00000001;
	

	Entity();
	Entity(uint32_t id);
	void SetHasTransform(bool b) { hasComponentBits |= ((b)?FLAG_TRANSFORM_COMPONENT : 0); }
	bool HasTransform() { return hasComponentBits & FLAG_TRANSFORM_COMPONENT; }
};

