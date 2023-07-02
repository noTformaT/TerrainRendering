#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>

#include "SkyBoxRenderSystem.h"
#include "Camera.h"


class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Init();
	
	void Render(Camera& camera, GLuint width, GLuint height);

	size_t GetCubemapIndex();
	void SetCubemapIndex(size_t index);
private:
	bool m_isInit = false;

	GLuint m_VBO = 0;
	GLuint m_VAO = 0;
	std::vector<GLint> m_textures;
	//GLuint m_cubemapTextureIndex;
	size_t m_cubemapIndex = 0;

	SkyBoxRenderSystem m_renderSystem;

	GLuint LoadCubemap(std::vector<std::string> faces);
};
