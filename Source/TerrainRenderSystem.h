#pragma once

#include "RenderSystem.h"
#include <glm/glm.hpp>

class TerrainRenderSystem : public RenderSystem
{
public:
	TerrainRenderSystem();
	
	virtual bool Init() override;
	void SetVP(const glm::mat4& vp);
	void SetMinMaxHeight(float min, float max);

private:
	GLuint m_VPLoc = -1;
	GLuint m_minHeightLoc = -1;
	GLuint m_maxHeightLoc = -1;
	GLuint m_myTextureLoc = -1;
};