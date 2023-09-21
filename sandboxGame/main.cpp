#define STB_IMAGE_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION


#include <core/window.h>
#include <render/shader.h>
#include <render/mesh.h>
#include <render/texture.h>
#include <utils/utils.h>
#include <Windows.h>

#include <ctime>

#include <time.h>
#include "stb_perlin.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <world/world.h>

#include <string>

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = -3.0f;

float cubeScaling = 1.0f;

int instances = 25;

bool wireframe = false;

struct IData {
	float fpsChart[100];
	float deltaTime[100];

	float dtSpacing = 0.0;

	float highestDT = 0.0f;
	int highestFPS = 0.0;

	Window* window;
	Shader* shader;

};

void guiHandler(void* pointer) {
	IData* data = ((IData*)pointer);


	glm::ivec2 wsize = data->window->getSize();

	
	float dt = data->window->getDelta();
	int fps = data->window->getFps();


	data->highestDT = 0.0f;
	for (int i = 0; i < 100; i++) {
		if (data->deltaTime[i] > data->highestDT) {
			data->highestDT = data->deltaTime[i];
		}

		if (data->fpsChart[i] > data->highestFPS) {
			data->highestFPS = data->fpsChart[i];
		}
	}

	bool open = true;

	ImGui::SetNextWindowSize(ImVec2(260, 400));

	ImGui::Begin("graphs");

	for (int i = 1; i < 100; i++) {
		data->deltaTime[i - 1] = data->deltaTime[i];
	}

	ImGui::PlotLines("", data->deltaTime, 100, 0, "dt", 0.0f, FLT_MAX, ImVec2(0, 50));
	ImGui::PlotLines("", data->fpsChart, 100, 0, "fps", 0.0f, FLT_MAX, ImVec2(0, 50));
	if (data->dtSpacing >= 1.0) {
		for (int i = 1; i < 100; i++) {
			data->fpsChart[i - 1] = data->fpsChart[i];
		}
		data->fpsChart[99] = fps;
		data->dtSpacing = 0.0;
	}

	data->deltaTime[99] = dt;
	data->dtSpacing += dt;


	ImGui::Dummy(ImVec2(0, 20));

	ImGui::DragFloat("x", &cameraX, 0.1f);
	ImGui::DragFloat("y", &cameraY, 0.1f);
	ImGui::DragFloat("z", &cameraZ, 0.1f);

	ImGui::Dummy(ImVec2(0, 20));

	ImGui::DragFloat("cube-scale", &cubeScaling, 0.5f, 0.0f);

	ImGui::Dummy(ImVec2(0, 20));



	ImGui::DragInt("instances", &instances, 1, 1000000);
	ImGui::Checkbox("wireframe", &wireframe);

	if (ImGui::Button("reload shader")) {
		(data->shader)->reload(readText("test.vert").c_str(), readText("test.frag").c_str());
	}


	ImGui::End();

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

int main() {
	WindowConfig conf;
	conf.vsync = true;
	conf.useImGui = true;
	conf.resizable = true;
	conf.imGuiHandler = guiHandler;

	IData ptr;


	memset(ptr.fpsChart, 0, sizeof(ptr.fpsChart));
	memset(ptr.deltaTime, 0, sizeof(ptr.fpsChart));


	Window window(800, 600, "testing", conf);



	ptr.window = &window;


	Shader shader(readText("test.vert").c_str(), readText("test.frag").c_str());

	ptr.shader = &shader;
	window.setUserPointer((void*)&ptr);

	float dt = 0.0f;

	Chunk chunks[RENDER_DISTANCE_X * RENDER_DISTANCE_Y];
	
	World world(&window, &shader);

	Texture texture("animation.gif", TEXTURE_ANIMATED);
	texture.bind(0);

	while (!window.shouldClose()) {

		dt += window.getDelta();

		if (dt >= (1.0f / texture.getFrameSize())) {
			texture.nextFrame(0);
			dt = 0.0f;
		}

		world.render();

		window.swapBuffers();
	}
}