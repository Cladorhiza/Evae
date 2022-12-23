#pragma once
#include <cstdint>
class Entity
{
private:
	uint32_t id;
	bool hasTransform;

public:

	Entity();
	Entity(uint32_t id);
	inline void SetHasTransform(bool b) { hasTransform = b; }
	inline bool HasTransform() { return hasTransform; }
};

