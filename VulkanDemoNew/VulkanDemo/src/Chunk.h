#pragma once
#include <vector>
#include "Vertex.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

class Chunk
{
	static constexpr unsigned CHUNK_SIZE = 8;
	static const unsigned BLOCK_AIR = 0;
	static const unsigned BLOCK_SOLID = 1;

private:
	unsigned ids[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	float pos[3];
	Chunk* neighbours[6];
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

public:
	inline std::vector<Vertex>& GetVertices() { return vertices; }
	inline std::vector<uint16_t>& GetIndices() { return indices; }
	void Init();
	void pushVertexes(int x, int y, int z, float xnorm, float ynorm, float znorm);
};

