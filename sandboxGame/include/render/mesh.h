#include <glad/glad.h>
#include <iostream>

#pragma once

struct Vertex {
	float position[3];
};


class Mesh {
public:
	Mesh() { vao = 0; vbo = 0; };
	Mesh(float vertices[], size_t vSize);

	void bind();
	void unbind();
private:
	unsigned int vao, vbo;
};