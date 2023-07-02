#pragma once

#include "RenderSystem.h"

#include <glm/glm.hpp>

class SkyBoxRenderSystem: public RenderSystem
{
public:
	SkyBoxRenderSystem();
	~SkyBoxRenderSystem();

	virtual bool Init() override;

	void UpdateViewProjection(glm::mat4& view, glm::mat4& projection);
private:
	GLint m_projectionLoc = -1;
	GLint m_viewLoc = -1;
};