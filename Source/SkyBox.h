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
	size_t GetCubemapCount();
	std::string GetCubemapName(size_t index);
	void SetCubemapIndex(size_t index);
private:
	struct TexturesData
	{
		std::string name;
		GLuint textureID;
	};

	bool m_isInit = false;

	GLuint m_VBO = 0;
	GLuint m_VAO = 0;
	std::vector<TexturesData> m_textures;
	//GLuint m_cubemapTextureIndex;
	size_t m_cubemapIndex = 0;

	SkyBoxRenderSystem m_renderSystem;

	void LoadCubemap(std::string name);
	GLuint LoadCubemaps(std::vector<std::string> faces);
};
