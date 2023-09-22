#include <GLAD/glad.h>
#include <iostream>

#pragma once

class Shader {
public:
	Shader(const char* vertexCode, const char* fragmentCode);

	void bind();
	void unbind();

	void reload(const char* vertexCode, const char* fragmentCode);

	unsigned int getProgam();
private:
	unsigned int program;
	unsigned int buildShader(const char* shaderCode, unsigned int type) const;

};