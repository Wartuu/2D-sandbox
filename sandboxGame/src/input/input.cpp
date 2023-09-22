#include <input/input.h>

Input* Input::instance = nullptr;

Input* Input::getInstance() {
	if (instance == nullptr) {
		instance = new Input();
	}

	return instance;
}

void Input::addWindow(Window& window) {
	GLFWwindow* handle = window.getHandle();

	windows.insert_or_assign(handle, window);

	glfwSetKeyCallback(handle, _keyCallback);
	glfwSetCursorPosCallback(handle, _cursorPosition);
	glfwSetCursorEnterCallback(handle, _cursorEnter);
	glfwSetScrollCallback(handle, _scrollCallback);
	glfwSetDropCallback(handle, _dropCallback);


}

void Input::setRawMouseMotion(bool enable) {
	for (const auto value : windows) {
		glfwSetInputMode(value.first, GLFW_RAW_MOUSE_MOTION, enable);
	}
}

void Input::setClipboardText(const char* text) {
	glfwSetClipboardString(NULL, text);
}

bool Input::isRawMotionSupported() {
	return glfwRawMouseMotionSupported();
}

bool Input::isKeyPressed(int key) {
	return keyboard.keys[key];
}

DropFiles Input::getDropFiles() {

	DropFiles returnData = dropFiles;

	dropFiles.count = 0;
	dropFiles.paths = nullptr;

	return dropFiles;
}

void Input::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::getInstance()->keyCallback(window, key, scancode, action, mods);
}

void Input::_cursorPosition(GLFWwindow* window, double xpos, double ypos) {
	Input::getInstance()->cursorPosition(window, xpos, ypos);
}

void Input::_cursorEnter(GLFWwindow* window, int entered) {
	Input::getInstance()->cursorEnter(window, entered);
}

void Input::_scrollCallback(GLFWwindow* window, double x, double y) {
	Input::getInstance()->scrollCallback(window, x, y);
}


void Input::_dropCallback(GLFWwindow* window, int count, const char** paths) {
	Input::getInstance()->dropCallback(window, count, paths);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	keyboard.keys[key] = (action != GLFW_RELEASE) ? true : false;

	if (action == GLFW_PRESS) {
		keyboard.lastKeyPressed = key;
	}

	if (action == GLFW_RELEASE) {
		if (keyboard.lastKeyPressed == key) {
			keyboard.lastKeyPressed = 0;
		}
	}

}

void Input::cursorPosition(GLFWwindow* window, double xpos, double ypos) {
	mouse.xPos = xpos;
	mouse.yPos = ypos;
}


void Input::cursorEnter(GLFWwindow* window, int entered) {
	currentWindow = (entered) ? window : nullptr;
}



void Input::scrollCallback(GLFWwindow* window, double x, double y) {
	mouse.scrollX = x;
	mouse.scrollY = y;
}

void Input::dropCallback(GLFWwindow* window, int count, const char** paths) {
	dropFiles.count = count;
	dropFiles.paths = paths;
}