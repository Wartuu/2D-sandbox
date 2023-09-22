#include <utils/utils.h>

std::string readText(const char* filename) {
	std::ifstream stream(filename);
	std::string output, line;

	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			output.append(line + "\n");
		}
	}

	stream.close();
	return output;

}

unsigned char* loadFile(const char* filename, int* size) {
	std::ifstream input(filename, std::ios::binary);

	input.seekg(0, std::ios::end);
	std::streampos fSize = input.tellg();
	(*size) = fSize;

	input.seekg(0, std::ios::beg);


	unsigned char* data = new unsigned char[fSize];
	input.read(reinterpret_cast<char*>(data), fSize);
	input.close();

	return data;
}
