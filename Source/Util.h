#pragma once

#include <string>

#define RANDOM rand

#define INVALID_UNIFORM_LOCATION 0xffffffff

char* ReadBinaryFile(const char* pFileName, int& size);

bool ReadFile(const char* pFileName, std::string& outFile);

int CalcNextPowerOfTwo(int x);

float RandomFloat();

float RandomFloatRange(float start, float end);

#define FOV 45.0f
#define NEAR_PLANE 0.1f