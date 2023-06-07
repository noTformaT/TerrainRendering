#pragma once

#include <glm/vec3.hpp>

class Image
{
public:
	Image();
	~Image();

	void Load(const char* pFileName);

	void Unload();

	glm::vec3 GetColor(int x, int y);

	int m_width = 0;
	int m_height = 0;
	int m_bpp = 0;
	unsigned char* m_imageData = nullptr;
private:

};