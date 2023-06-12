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

	GLuint m_myTexture1Loc = -1;
	GLuint m_myTexture2Loc = -1;
	GLuint m_myTexture3Loc = -1;
	GLuint m_myTexture4Loc = -1;

	GLuint m_tex0HeightLoc = -1;
	GLuint m_tex1HeightLoc = -1;
	GLuint m_tex2HeightLoc = -1;
	GLuint m_tex3HeightLoc = -1;
};