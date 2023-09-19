#include <iostream>

#pragma once

enum ERROR_TYPE : int {
	// GLFW ERRORS
	GLFW_INIT_FAIL = 1,
	GLFW_WINDOW_FAIL,


	//OPENGL ERRORS
	GLAD_INIT_FAIL,

	// IMGUI ERRORS
	IMGUI_INIT_FAIL,

	SHADER_COMPILATION_FAIL,
	SHADER_LINKER_FAIL,

	WRONG_TEXTURE_TYPE

};

namespace util {
	void throwError(ERROR_TYPE error);
}




