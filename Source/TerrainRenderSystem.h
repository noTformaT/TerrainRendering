#pragma once

#include "RenderSystem.h"
#include <glm/glm.hpp>

class TerrainRenderSystem : public RenderSystem
{
public:
	TerrainRenderSystem();
	
	virtual bool Init() override;
	void SetVP(const glm::mat4& vp);

private:
	GLuint m_VPLoc = -1;
};