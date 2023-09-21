#include <glad/glad.h>

#include <utils/error.h>
#include <utils/utils.h>

#include "stb_image.h"

#pragma once

enum texture_type {
	TEXTURE_ANIMATED,
	TEXTURE_STATIC
};


class Texture {
public:
	Texture() {};
	Texture(const char* filename, texture_type type);

	void bind(int id);
	void unbind();

	int getFrameSize();


	// only used for TEXTURE_ANIMATED
	void nextFrame(int id);


private:
	texture_type type;

	unsigned int* textures;

	void loadAnimated(const char* filename);
	void loadStatic(const char* filename);

	int frameSize;
	int currentFrame;
};