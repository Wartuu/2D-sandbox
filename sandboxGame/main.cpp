#define STB_IMAGE_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION


#include <core/window.h>
#include <render/shader.h>
#include <render/mesh.h>
#include <render/texture.h>
#include <utils/utils.h>

#include <time.h>
#include "stb_perlin.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = -3.0f;

float cameraXlen;
float cameraYlen;

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

	ImGui::DragFloat("ortho-x", &cameraXlen, 1.0f, 0.0f);
	ImGui::DragFloat("ortho-y", &cameraYlen, 1.0f, 0.0f);

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
	
	float vertices[] = {
		0.0f, 0.0f,   0, 0, // down-left (adjusted for ortho)
		0.0f, 20.0f,  0, 1, // top-left (adjusted for ortho)
		20.0f, 20.0f, 1, 1,  // top-right (adjusted for ortho)
		20.0f, 0.0f,  1, 0  // down-right (adjusted for ortho)
	};

	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};

	WindowConfig conf;
	conf.vsync = true;
	conf.useImGui = true;
	conf.imGuiHandler = guiHandler;

	IData ptr;

	for (int i = 0; i < 100; i++) {
		ptr.fpsChart[i] = 0;
	}

	for (int i = 0; i < 100; i++) {
		ptr.deltaTime[i] = 0.0;
	}

	Window window(800, 600, "testing", conf);



	ptr.window = &window;

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	mesh.bind();

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indicies, GL_STATIC_DRAW);

	cameraXlen = (float)window.getSize().x;
	cameraYlen = (float)window.getSize().y;

	glm::mat4 proj = glm::ortho(0.0f, cameraXlen, 0.0f, cameraYlen, 0.01f, 1000.0f);


	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);



	auto radians = glm::radians((float)glfwGetTime());
	view = glm::translate(view, glm::vec3(1, 1, -100));

	Shader shader(readText("test.vert").c_str(), readText("test.frag").c_str());
	shader.bind();
	unsigned int shaderID = shader.getProgam();

	Texture texture("animation.gif", TEXTURE_ANIMATED);
	texture.bind(0);


	ptr.shader = &shader;
	window.setUserPointer((void*)&ptr);

	float dt = 0.0f;
	while (!window.shouldClose()) {
		dt += window.getDelta();

		if (dt >= (1.0f / texture.getFrameSize())) {
			texture.nextFrame(0);
			dt = 0.0f;
		}
		

		radians = glm::radians(glfwGetTime());

		view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
		proj = glm::ortho(0.0f, cameraXlen, 0.0f, cameraYlen, 0.01f, 1000.0f);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(cubeScaling, cubeScaling, 1.0f));
		model = glm::rotate(model, radians, glm::vec3(0.f, 1.f, 0.f));

#pragma region SHADER_BINDING


		glUniformMatrix4fv(
			glGetUniformLocation(shaderID, "view"),
			1, false, glm::value_ptr(view)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(shaderID, "proj"),
			1, false, glm::value_ptr(proj)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(shaderID, "model"),
			1, false, glm::value_ptr(model)
		);

		glUniform1f(
			glGetUniformLocation(shaderID, "iTime"),
			(float)glfwGetTime()
		);

		glUniform1i(
			glGetUniformLocation(shaderID, "tx0"),
			0
		);

#pragma endregion



		glClearColor(0.2f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances);

		window.swapBuffers();



	}
}