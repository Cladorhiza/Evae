#include "Terrain.h"

void Terrain::UpdateChunkLoadQueues(glm::vec3 playerPosition, int drawDistance) {

	//find chunk loading bounds
	glm::vec3 positiveBound(playerPosition.x + drawDistance, playerPosition.y + drawDistance, playerPosition.z + drawDistance);
	glm::vec3 negativeBound(playerPosition.x - drawDistance, playerPosition.y - drawDistance, playerPosition.z - drawDistance);

	//iterate chunks, if chunk index outside bounds, add to unload queue
	for (const std::pair<glm::vec3, Chunk>& c : loadedChunks) {
		if (   c.first.x < negativeBound.x && c.first.y < negativeBound.y && c.first.z < negativeBound.z
			|| c.first.x > positiveBound.x && c.first.y > positiveBound.y && c.first.z > positiveBound.z) {
			unloadQueue.push(c.first);
		}
	}
	//iterate bounds, if chunk index inside bounds AND chunk not loaded already, add to load queue
	for (int x = negativeBound.x; x < positiveBound.x; x += Chunk::CHUNK_SIZE) {
		for (int y = negativeBound.y; x < positiveBound.y; x += Chunk::CHUNK_SIZE) {
			for (int z = negativeBound.z; x < positiveBound.z; x += Chunk::CHUNK_SIZE) {

				glm::vec3 index(x / Chunk::CHUNK_SIZE, y / Chunk::CHUNK_SIZE, z / Chunk::CHUNK_SIZE);
				if (loadedChunks.find(index) == loadedChunks.end()) {
					loadQueue.push(index);
				}
			}
		}
	}


}

void Terrain::LoadQueue() {
	while (!loadQueue.empty()) {
		glm::vec3 index(loadQueue.front());
		loadedChunks.emplace(index);
		loadedChunks[index].Init(index.x * 16, index.y * 16, index.z * 16);
		//still doesn't link nearby chunks
		//still doesn't construct vbo/ibo or store any of them
		loadQueue.pop();
	}

}

void Terrain::UnLoadQueue() {


}