#pragma once

#include <string>

#include <GL/glew.h>

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string& fileName);
	Texture(GLenum textureTarget);

	bool Load();

	void Load(unsigned int bufferSize, void* pData);

	void Load(const std::string& Filename);

	void LoadRaw(int width, int height, int bpp, unsigned char* pData);

	void Bind(GLenum textureUnit);

	void GetImageSize(int& imageWidth, int& imageHeight);

	GLuint GetTexture() const;

	~Texture();

private:
	void LoadInternal(void* image_data);

	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_imageBPP = 0;

};