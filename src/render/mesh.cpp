#include <render/mesh.h>

Mesh::Mesh(float vertices[], size_t vSize) {

	// init just to get rid of warns;
	vao = 0;
	vbo = 0;


	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

}

void Mesh::bind() {
	glBindVertexArray(vao);
}

void Mesh::unbind() {
	glBindVertexArray(0);
}
