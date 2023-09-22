#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <render/texture.h>
#include <render/shader.h>

#include <vector>


// temp usage for testing
#include <core/window.h>



#pragma once

#define CHUNK_SIZE_X 100
#define CHUNK_SIZE_Y 100
#define BLOCK_SIZE 16

#define RENDER_DISTANCE_Y 5
#define RENDER_DISTANCE_X 5

#define MAX_ZOOM 2.0
#define MIN_ZOOM 0.5

class Chunk;
class World;
struct Vertex;
struct MVP;


struct MVP {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	unsigned int modelUniform;
	unsigned int viewUniform;
	unsigned int projectionUniform;
};

class Chunk {
public:
	Chunk(int x, int y, int seed);
	Chunk() {};

	void render();

private:
	int x;
	int y;
	int seed;

	unsigned int vao;
	unsigned int vbo;
};

class World {
public:
	World(Window* window, Shader* blockShader);

	void render();


	void setCameraPos(float x, float y);
	glm::vec2 getCameraPos();

private:
	float cameraX = 0.0f;
	float cameraY = 0.0f;
	float zoom = 1.0f;



	MVP mvp;
	Chunk* chunks;
	Window* window;
	Shader* blockShader;


};

struct Vertex {
	glm::vec2 pos;
	glm::vec2 texcoord;
};