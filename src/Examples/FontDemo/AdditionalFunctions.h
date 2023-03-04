#pragma once
#include <vector>
#include <fstream>

using namespace std;

std::vector<char> readFile(const std::string& filename);
unsigned char* readJPG(const std::string& filename, int &width, int &height, int &channels);
void freeImage(unsigned char* pixels);
unsigned char* GenerateFontSet(const int fontsize, int& width, int& height);