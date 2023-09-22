#include <render/Shader.h>



void shaderStatus(unsigned int shader) {
	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		int messageSize = 0;
		char message[1024];

		glGetShaderInfoLog(shader, 1024, &messageSize, message);
		std::cout << message << '\n';
	}
}

void programStatus(unsigned int program) {
	int compiled;
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);

	if (!compiled) {
		int messageSize = 0;
		char message[1024];

		glGetProgramInfoLog(program, 1024, &messageSize, message);
		std::cout << message << '\n';

	}
}



Shader::Shader(const char* vertexCode, const char* fragmentCode) {
	unsigned int vertexShader = buildShader(vertexCode, GL_VERTEX_SHADER);
	unsigned int fragmentShader = buildShader(fragmentCode, GL_FRAGMENT_SHADER);



	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	programStatus(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::bind() {
	glUseProgram(program);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::reload(const char* vertexCode, const char* fragmentCode) {
	glUseProgram(0);
	glDeleteProgram(program);

	unsigned int vertexShader = buildShader(vertexCode, GL_VERTEX_SHADER);
	unsigned int fragmentShader = buildShader(fragmentCode, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	programStatus(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(program);

}

unsigned int Shader::getProgam() {
	return program;
}

unsigned int Shader::buildShader(const char* shaderCode, unsigned int type) const {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, 0);
	glCompileShader(shader);
	shaderStatus(shader);
	return shader;
}
