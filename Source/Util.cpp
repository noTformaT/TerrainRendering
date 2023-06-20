#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>

char* ReadBinaryFile(const char* pFilename, int& size)
{
    FILE* f = NULL;

    errno_t err = fopen_s(&f, pFilename, "rb");

    if (!f) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        printf("Error opening '%s': %s\n", pFilename, buf);
        exit(0);
    }

    struct stat stat_buf;
    int error = stat(pFilename, &stat_buf);

    if (error) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        printf("Error getting file stats: %s\n", buf);
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    size_t bytes_read = fread(p, 1, size, f);

    if (bytes_read != size) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        printf("Read file error file: %s\n", buf);
        exit(0);
    }

    fclose(f);

    return p;
}

bool ReadFile(const char* pFileName, std::string& outFile)
{
    std::ifstream f(pFileName);

    bool ret = false;

    if (f.is_open()) {
        std::string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();

        ret = true;
    }
    else {
        printf(pFileName);
    }

    return ret;
}

int CalcNextPowerOfTwo(int x)
{
    int ret = 1;

    if (x == 1)
    {
        return 2;
    }

    while (ret < x)
    {
        ret = ret * 2;
    }

    return ret;
}

float RandomFloat()
{
    float max = RAND_MAX;
    return ((float)RANDOM() / max);
}

float RandomFloatRange(float start, float end)
{
    float delta = end - start;

    float randomValue = RandomFloat() * delta + start;

    return randomValue;
}
