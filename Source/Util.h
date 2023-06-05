#pragma once

#include <string>

#define INVALID_UNIFORM_LOCATION 0xffffffff

char* ReadBinaryFile(const char* pFileName, int& size);

bool ReadFile(const char* pFileName, std::string& outFile);