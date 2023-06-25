#pragma once

#include "RenderSystem.h"
#include <glm/glm.hpp>

struct LightingData;

class TerrainRenderSystem : public RenderSystem
{
public:
	TerrainRenderSystem();
	
	virtual bool Init() override;
	void SetVP(const glm::mat4& vp);
	void SetMinMaxHeight(float min, float max);
	void SetLevels(float l0, float l1, float l2, float l3, float l4, float l5);
	void SetLightingData(LightingData& lightingData);

private:
	GLuint m_VPLoc = 0;
	GLuint m_minHeightLoc = 0;
	GLuint m_maxHeightLoc = 0;

	GLuint m_myTexture1Loc = 0;
	GLuint m_myTexture2Loc = 0;
	GLuint m_myTexture3Loc = 0;
	GLuint m_myTexture4Loc = 0;
	GLuint m_myTexture5Loc = 0;

	GLuint m_level0 = 0;
	GLuint m_level1 = 0;
	GLuint m_level2 = 0;
	GLuint m_level3 = 0;
	GLuint m_level4 = 0;
	GLuint m_level5 = 0;

	GLuint m_sunDirectionLoc = 0;
	GLuint m_sunColorLoc = 0;
	GLuint m_sunIntencityLoc = 0;
	GLuint m_sunDiffuse = 0;
	GLuint m_useLit = 0;
};