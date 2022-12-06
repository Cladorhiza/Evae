#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "Chunk.h"

#include <unordered_map>
#include <queue>

class Terrain
{


private:
	std::unordered_map <glm::vec3, Chunk> loadedChunks;
	std::queue<glm::vec3> loadQueue;
	std::queue<glm::vec3> unloadQueue;
	
public:
	void UpdateChunkLoadQueues(glm::vec3 playerPosition, int drawDistance);
	void LoadQueue();
	void UnLoadQueue();
};

