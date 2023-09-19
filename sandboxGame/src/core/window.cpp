#include <core/window.h>

Window::Window(int width, int height, const char* title, WindowConfig config) :
	width(width), height(height), title(title), config(config) {

	if (!glfwInit())
		util::throwError(GLFW_INIT_FAIL);

	lastTimeDelta = glfwGetTime();
	lastTimeFps = glfwGetTime();

	deltaTime = 0.0f;
	fps = 0;
	fpsCounter = 0;



	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, config.resizable);
	glfwWindowHint(GLFW_SAMPLES, config.samples);
	glfwWindowHint(GLFW_VISIBLE, false);

	handle = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!handle)
		util::throwError(GLFW_WINDOW_FAIL);

	glfwMakeContextCurrent(handle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		util::throwError(GLAD_INIT_FAIL);

	glfwSwapInterval(config.vsync);
	glViewport(0, 0, width, height);

	if (config.useImGui == false) {
		glfwShowWindow(handle);
		return;
	}

	ImGui::CreateContext();
	guiIO = ImGui::GetIO();


	ImGui_ImplGlfw_InitForOpenGL(handle, true);
	ImGui_ImplOpenGL3_Init(config.imGuiShaderVersion);
	
	guiIO.IniFilename = 0;
	guiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	guiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	guiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	
	glfwShowWindow(handle);
}

glm::ivec2 Window::getSize() {
	return glm::ivec2(width, height);
}

const char* Window::getTitle() {
	return title;
}

int Window::getFps() {
	return fps;
}

double Window::getDelta() {
	return deltaTime;
}

void Window::setSize(int width, int height) {
	this->width = width;
	this->height = height;

	glfwSetWindowSize(handle, width, height);
	glViewport(0, 0, width, height);
}

void Window::setSize(glm::ivec2 size) {
	this->width = size.x;
	this->height = size.y;
	glfwSetWindowSize(handle, size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}

void Window::setTitle(const char* title) {
	this->title = title;
	glfwSetWindowTitle(handle, title);
}

void Window::setVsync(bool enable) {
	config.vsync = enable;
	glfwSwapInterval(enable);
}

void Window::setUserPointer(void* ptr) {
	config.userPointer = ptr;
}

void* Window::getUserPointer() {
	return config.userPointer;
}

bool Window::isVsync() {
	return config.vsync;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(handle);
}

void Window::swapBuffers() {
	glfwPollEvents();

	double time = glfwGetTime();

	deltaTime = time - lastTimeDelta;

	if (time <= lastTimeFps + 1.0) {
		fpsCounter++;
	}
	else {
		fps = fpsCounter;
		fpsCounter = 0;
		lastTimeFps = time;
	}

	if (config.useImGui) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		config.imGuiHandler(config.userPointer);
		 
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	glfwSwapBuffers(handle);
	lastTimeDelta = time;
}

GLFWwindow* Window::getHandle() {
	return handle;
}




