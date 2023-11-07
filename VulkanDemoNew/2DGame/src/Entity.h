#pragma once
#include <cstdint>
#include <string>
class Entity
{
private:
	uint32_t id;
	uint32_t hasComponentBits;
	std::string tag;

public:

	//whether or not an entity has x component
	static const uint32_t FLAG_TRANSFORM_COMPONENT = 0x00000001;
	static const uint32_t FLAG_SPRITE_COMPONENT = 0x00000002;
	
	

	Entity();
	Entity(uint32_t id);
	Entity(uint32_t id, std::string tag);

	void SetHasTransform(bool b) { hasComponentBits |= ((b)?FLAG_TRANSFORM_COMPONENT : 0); }
	bool HasTransform() { return hasComponentBits & FLAG_TRANSFORM_COMPONENT; }
	void SetHasSprite(bool b) { hasComponentBits |= ((b) ? FLAG_SPRITE_COMPONENT : 0); }
	bool HasSprite() { return hasComponentBits & FLAG_SPRITE_COMPONENT; }
	void SetTag(std::string t) { tag = t; }
	std::string GetTag() { return tag; }
	
};

