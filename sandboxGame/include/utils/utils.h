#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#pragma once

std::string readText(const char* filename);
unsigned char* loadFile(const char* filename, int* size);