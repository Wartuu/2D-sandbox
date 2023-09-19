#include <render/particle.h>



ParticleEngine::ParticleEngine(Window* window, int maxParticles) :
	window(window), maxParticles(maxParticles) {

	glm::ivec2 windowSize = (*window).getSize();
	glm::translate(view, glm::vec3(0, 0, -3));
	view = glm::ortho(0, 0, windowSize.x, windowSize.y);

}

void ParticleEngine::addParticle(Particle particle) {
	if (particles.size() < maxParticles)
		particles.push_back(particle);
}

void ParticleEngine::render(double dt, Shader shader) {
	shader.bind();
	unsigned int mvpPositions = glGetUniformLocation(shader.getProgam(), "mvp");

	for (auto particle = particles.begin(); particle != particles.end(); particle++) {
		if (particle->lifetime <= 0.0f) {
			particles.erase(particle);
			continue;
		}



	}

}
