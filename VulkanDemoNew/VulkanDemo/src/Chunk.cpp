#include "Chunk.h"

void Chunk::Init() {

	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				ids[i][j][k] = (i + j + k) % 2;



			}
		}
	}

	for (int i = 1; i < CHUNK_SIZE-1; i++) {
		for (int j = 1; j < CHUNK_SIZE-1; j++) {
			for (int k = 1; k < CHUNK_SIZE-1; k++) {

				//what am i, if air stop.
				if (ids[i][j][k] == BLOCK_AIR) continue;

				//get 6 faces solid or not around me
				if (ids[i-1][j][k] == BLOCK_AIR) pushVertexes(i, j, k,-0.5f, 0.0f, 0.0f);
				if (ids[i+1][j][k] == BLOCK_AIR) pushVertexes(i, j, k,0.5f, 0.0f, 0.0f);
				if (ids[i][j-1][k] == BLOCK_AIR) pushVertexes(i, j, k,0.0f, -0.5f, 0.0f);
				if (ids[i][j+1][k] == BLOCK_AIR) pushVertexes(i, j, k,0.0f, 0.5f, 0.0f);
				if (ids[i][j][k-1] == BLOCK_AIR) pushVertexes(i, j, k,0.0f, 0.0f, -0.5f);
				if (ids[i][j][k+1] == BLOCK_AIR) pushVertexes(i, j, k,0.0f, 0.0f, 0.5f);




			}
		}
	}

	std::cout << "size of vertices: " << vertices.size() << ". size of indices: " << indices.size() << std::endl;
}

void Chunk::pushVertexes(int x, int y, int z, float xnorm, float ynorm, float znorm) {




	int size = vertices.size();

	if (xnorm > 0) {
		//right
		Vertex v1{ {1.0f + x, 0.0f + y, 1.0f + z}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };
		Vertex v2{ {1.0f + x, 0.0f + y, 0.0f + z}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };
		Vertex v3{ {1.0f + x, 1.0f + y, 0.0f + z}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
		Vertex v4{ {1.0f + x, 1.0f + y, 1.0f + z}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	else if (xnorm < 0) {
		//left
		Vertex v1{ {0.0f + x, 0.0f + y, 0.0f + z}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
		Vertex v2{ {0.0f + x, 0.0f + y, 1.0f + z}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
		Vertex v3{ {0.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
		Vertex v4{ {0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	else if (ynorm < 0) {
		//bottom
		Vertex v1{ {1.0f + x, 0.0f + y, 1.0f + z}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		Vertex v2{ {0.0f + x, 0.0f + y, 1.0f + z}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };
		Vertex v3{ {0.0f + x, 0.0f + y, 0.0f + z}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		Vertex v4{ {1.0f + x, 0.0f + y, 0.0f + z}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	else if (ynorm > 0) {
		//top
		Vertex v1{ {0.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} };
		Vertex v2{ {1.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
		Vertex v3{ {1.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };
		Vertex v4{ {0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	else if (znorm > 0) {
		//back
		Vertex v1{ {0.0f + x, 0.0f  + y, 1.0f  + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} };
		Vertex v2{ {1.0f + x, 0.0f  + y, 1.0f  + z}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} };
		Vertex v3{ {1.0f + x, 1.0f + y, 1.0f + z}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} };
		Vertex v4{ {0.0f + x, 1.0f + y, 1.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	else if (znorm < 0) {
		//front
		Vertex v1{ {1.0f + x, 0.0f  + y, 0.0f  + z}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		Vertex v2{ {0.0f + x, 0.0f  + y, 0.0f  + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };
		Vertex v3{ {0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		Vertex v4{ {1.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
	}
	
	indices.push_back(size + 3);
	indices.push_back(size + 0);
	indices.push_back(size + 1);
	indices.push_back(size + 3);
	indices.push_back(size + 1);
	indices.push_back(size + 2);

}