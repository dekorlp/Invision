#ifndef ADDITIONAL_FUNCTIONS_H
#define ADDITIONAL_FUNCTIONS_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include "Vertex.h"

using namespace std;

std::vector<char> readFile(const std::string& filename);
unsigned char* readPNG(const std::string& filename, int &width, int &height, int &channels);
void freeImage(unsigned char* pixels);

void LoadModel(const std::string& filename, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

#endif