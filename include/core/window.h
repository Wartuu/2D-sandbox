#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <utils/error.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <map>

#pragma once

class WindowCallbacks;
class Window;
struct WindowConfig;


class WindowCallbacks {
public:
	WindowCallbacks(const WindowCallbacks&) = delete;
	WindowCallbacks& operator=(const WindowCallbacks&) = delete;

	static WindowCallbacks* getInstance();

	void addWindow(Window& window);


private:
	WindowCallbacks() {};

	std::map<GLFWwindow*, Window&> windows;

	void frameBufferCallback(GLFWwindow* window, int width, int height);
	static void _frameBufferCallback(GLFWwindow* window, int width, int height);


	static WindowCallbacks* instance;



};

struct WindowConfig {
	bool resizable = false;
	bool vsync = false;
	bool antialiasing = false;
	int samples = 4;

	bool debug = false;

	int glMajor = 4;
	int glMinor = 3;


	bool useImGui = false;
	const char* imGuiShaderVersion = "#version 430";
	void(*imGuiHandler)(void*) = nullptr;
	void* userPointer = nullptr;
};

class Window {
public:
	Window(int width, int height, const char* title, WindowConfig config = WindowConfig{});

	glm::ivec2 getSize();
	const char* getTitle();
	int getFps();
	double getDelta();

	void setSize(int width, int height);
	void setSize(glm::ivec2 size);

	void setTitle(const char* title);
	void setVsync(bool enable);

	void setUserPointer(void* ptr);
	void* getUserPointer();


	bool isVsync();

	bool shouldClose();
	void swapBuffers();

	GLFWwindow* getHandle();



private:
	int width;
	int height;
	const char* title;

	double lastTimeDelta;
	double lastTimeFps;
	double deltaTime;
	int fps;
	int fpsCounter;

	GLFWwindow* handle = nullptr;
	ImGuiIO guiIO;

	WindowConfig config;
	WindowCallbacks* callbacks = nullptr;
};

