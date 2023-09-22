#include <world/world.h>

Chunk::Chunk(int x, int y, int seed) : x(x), y(y), seed(seed) {
	vao = 0;
	vbo = 0;

	Vertex* meshData = new Vertex[CHUNK_SIZE_X * CHUNK_SIZE_Y * 6];

	int counter = 0;
	float halfSize = BLOCK_SIZE / 2.0;


	for (int y = 0; y < CHUNK_SIZE_Y; y++) {
		for (int x = 0; x < CHUNK_SIZE_X; x++) {
			Vertex v1;
			v1.pos.x = (x * BLOCK_SIZE) - halfSize;
			v1.pos.y = (y * BLOCK_SIZE) - halfSize;
			v1.texcoord.x = 0.2f;
			v1.texcoord.y = 0.0f;

			Vertex v2;
			v2.pos.x = (x * BLOCK_SIZE) - halfSize;
			v2.pos.y = (y * BLOCK_SIZE) + halfSize;
			v2.texcoord.x = 0.0f;
			v2.texcoord.y = 1.0f;


			Vertex v3;
			v3.pos.x = (x * BLOCK_SIZE) + halfSize;
			v3.pos.y = (y * BLOCK_SIZE) + halfSize;
			v3.texcoord.x = 1.0f;
			v3.texcoord.y = 1.0f;

			Vertex v4;

			v4.pos.x = (x * BLOCK_SIZE) + halfSize;
			v4.pos.y = (y * BLOCK_SIZE) - halfSize;
			v4.texcoord.x = 1.0f;
			v4.texcoord.y = 0.0f;

			meshData[counter++] = v1;
			meshData[counter++] = v2;
			meshData[counter++] = v3;
			meshData[counter++] = v3;
			meshData[counter++] = v4;
			meshData[counter++] = v1;

		}

	}


	for (int i = 0; i < CHUNK_SIZE_X * CHUNK_SIZE_Y * 6; i++) {
		meshData[i].pos.x += this->x * (BLOCK_SIZE + 1.0) * CHUNK_SIZE_X;
		meshData[i].pos.y += this->y * (BLOCK_SIZE + 1.0) * CHUNK_SIZE_Y;
	}



	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int sizeData = CHUNK_SIZE_X * CHUNK_SIZE_Y * 6;
	glBufferData(GL_ARRAY_BUFFER, sizeData * sizeof(Vertex), meshData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	free(meshData);

}

void Chunk::render() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, CHUNK_SIZE_X * CHUNK_SIZE_Y * 6);

}


World::World(Window* window, Shader* blockShader) : window(window), blockShader(blockShader) {
	chunks = new Chunk[RENDER_DISTANCE_X * RENDER_DISTANCE_Y];

	for (int y = 0; y < RENDER_DISTANCE_Y; y++) {
		for (int x = 0; x < RENDER_DISTANCE_X; x++) {
			chunks[y * RENDER_DISTANCE_Y + x] = Chunk(x, y, 0);
		}
	}

	glm::ivec2 windowSize = window->getSize();
	int seed = (int)(window->getDelta() * 100000);
	unsigned int shaderID = blockShader->getProgam();


	mvp.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	mvp.modelUniform = glGetUniformLocation(shaderID, "model");

	mvp.projection = glm::ortho(0.0f, (float) windowSize.x, 0.0f, (float) windowSize.y, 0.1f, 1000.0f);
	mvp.projectionUniform = glGetUniformLocation(shaderID, "proj");
	
	mvp.view = glm::translate(glm::mat4(1), glm::vec3(zoom, zoom, 1.0f));
	mvp.viewUniform = glGetUniformLocation(shaderID, "view");

}

void World::render() {
	glClearColor(0.2f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	blockShader->bind();

	glUniformMatrix4fv(mvp.modelUniform, 1, GL_FALSE, glm::value_ptr(mvp.model));
	glUniformMatrix4fv(mvp.viewUniform, 1, GL_FALSE, glm::value_ptr(mvp.view));
	glUniformMatrix4fv(mvp.projectionUniform, 1, GL_FALSE, glm::value_ptr(mvp.projection));

	for (int i = 0; i < RENDER_DISTANCE_X * RENDER_DISTANCE_Y;  i++) {
		chunks[i].render();
	}

}