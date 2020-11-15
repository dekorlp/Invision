#include "AdditionalFunctions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}


unsigned char* readJPG(const std::string& filename, int &width, int &height, int &channels)
{
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	uint32_t imageSize = width * height * 4;
	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}


	return pixels;
}