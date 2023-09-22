#include <core/window.h>
#include <GLFW/glfw3.h>

#include <map>

#pragma once

// todo:
// controller support
// keyboard, mouse


struct DropFiles {
	int count = 0;
	const char** paths;
};

struct Mouse {
	double scrollX = 0.0;
	double scrollY = 0.0;

	double xPos = 0.0;
	double yPos = 0.0;
};

struct Keyboard {
	bool keys[1024];

	int lastKeyPressed;
};

class Input {
public:
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	static Input* getInstance();

	void addWindow(Window& window);

	void setRawMouseMotion(bool enable);
	void setClipboardText(const char* text);


	bool isRawMotionSupported();
	bool isKeyPressed(int key);
	DropFiles getDropFiles();

private:
	Input() {};

	Mouse mouse;
	Keyboard keyboard;
	DropFiles dropFiles;
	GLFWwindow* currentWindow;

	std::map<GLFWwindow*, Window&> windows;


	static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _cursorPosition(GLFWwindow* window, double xpos, double ypos);
	static void _cursorEnter(GLFWwindow* window, int entered);
	static void _scrollCallback(GLFWwindow* window, double x, double y);
	static void _dropCallback(GLFWwindow* window, int count, const char** paths);


	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorPosition(GLFWwindow* window, double xpos, double ypos);
	void cursorEnter(GLFWwindow* window, int entered);
	void scrollCallback(GLFWwindow* window, double x, double y);
	void dropCallback(GLFWwindow* window, int count, const char** paths);

	// todo: gamepad, joystick


	static Input* instance;
};