#include <render/texture.h>




Texture::Texture(const char* filename, texture_type type) {
	this->type = type;

	stbi_set_flip_vertically_on_load(true);

	if (type == TEXTURE_ANIMATED)
		loadAnimated(filename);
	else
		loadStatic(filename);

}

void Texture::bind(int id) {

	if (TEXTURE_STATIC) {
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}
	else {
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, textures[currentFrame]);
	}

}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	currentFrame = 0;
}

int Texture::getFrameSize(){
	return frameSize;
}

void Texture::nextFrame(int id) {
	if (type != TEXTURE_ANIMATED)
		util::throwError(WRONG_TEXTURE_TYPE);
	
	currentFrame++;

	if (currentFrame > frameSize - 1) {
		currentFrame = 0;
	}

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, textures[currentFrame]);

}

void Texture::loadAnimated(const char* filename) {
	int size;
	int x;
	int y;
	int comp;

	unsigned char* raw = loadFile(filename, &size);

	unsigned char* image = stbi_load_gif_from_memory(
		raw, size,
		NULL,
		&x, &y, &frameSize,
		&comp, 0
		);


	textures = new unsigned int[frameSize];
	glGenTextures(frameSize, textures);

	for (int i = 0; i < frameSize; i++) {
		int offset = i * x * y * comp;

		unsigned char* frameData = image + offset;


		int format = (comp == 4) ? GL_RGBA : GL_RGB;

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, frameData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(image);
}

void Texture::loadStatic(const char* filename){
	int x;
	int y;
	int comp;

	frameSize = -1;

	unsigned char* image = stbi_load(filename, &x, &y, &comp, 0);

	textures = new unsigned int[1];

	glGenTextures(1, textures);

	int format = (comp == 4) ? GL_RGBA : GL_RGB;

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
}
