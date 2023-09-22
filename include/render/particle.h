#include <render/mesh.h>
#include <core/window.h>
#include <render/shader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#pragma once

struct Particle {
	glm::vec2 position = glm::vec2(0,0);
	glm::vec3 movement = glm::vec3(0, 0, 0);
	glm::vec4 color    = glm::vec4(0, 0, 0, 0);
	
	double lifetime     = 10.0f;
};


class ParticleEngine {
public:
	ParticleEngine(Window* window, int maxParticles);

	void addParticle(Particle particle);
	void render(double dt, Shader shader);
private:

	Window* window;
	int maxParticles;
	std::vector<Particle> particles;

	glm::mat4 projection;
	glm::mat4 view;

};