#include "TerrainRenderSystem.h"
#include "Util.h"
#include <glm/gtc/type_ptr.hpp>

TerrainRenderSystem::TerrainRenderSystem()
{
}

bool TerrainRenderSystem::Init()
{
	if (!RenderSystem::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/terrain.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/terrain.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	m_VPLoc = GetUniformLocation("model");

	if (m_VPLoc == INVALID_UNIFORM_LOCATION)
	{
		//return false;
	}

	m_minHeightLoc = GetUniformLocation("minHeight");
	if (m_minHeightLoc == INVALID_UNIFORM_LOCATION)
	{
		//return false;
	}

	m_maxHeightLoc = GetUniformLocation("maxHeight");
	if (m_minHeightLoc == INVALID_UNIFORM_LOCATION)
	{
		//return false;
	}

	m_myTexture1Loc = GetUniformLocation("myTexture1");
	m_myTexture2Loc = GetUniformLocation("myTexture2");
	m_myTexture3Loc = GetUniformLocation("myTexture3");
	m_myTexture4Loc = GetUniformLocation("myTexture4");
	m_myTexture5Loc = GetUniformLocation("myTexture5");
	if (m_myTexture1Loc == INVALID_UNIFORM_LOCATION || 
		m_myTexture2Loc == INVALID_UNIFORM_LOCATION ||
		m_myTexture3Loc == INVALID_UNIFORM_LOCATION ||
		m_myTexture4Loc == INVALID_UNIFORM_LOCATION)
	{
		//return false;
	}

	m_level0 = GetUniformLocation("gHeight0");
	m_level1 = GetUniformLocation("gHeight1");
	m_level2 = GetUniformLocation("gHeight2");
	m_level3 = GetUniformLocation("gHeight3");
	m_level4 = GetUniformLocation("gHeight4");
	m_level5 = GetUniformLocation("gHeight5");

	Enable();

	glUniform1i(m_myTexture1Loc, 0);
	glUniform1i(m_myTexture2Loc, 1);
	glUniform1i(m_myTexture3Loc, 2);
	glUniform1i(m_myTexture4Loc, 3);
	glUniform1i(m_myTexture5Loc, 4);

	return true;
}

void TerrainRenderSystem::SetVP(const glm::mat4& vp)
{
	glUniformMatrix4fv(m_VPLoc, 1, GL_FALSE, glm::value_ptr(vp));
}

void TerrainRenderSystem::SetMinMaxHeight(float min, float max)
{
	glUniform1f(m_minHeightLoc, min);
	glUniform1f(m_maxHeightLoc, max);
}

void TerrainRenderSystem::SetLevels(float l0, float l1, float l2, float l3, float l4, float l5)
{
	glUniform1f(m_level0, l0);
	glUniform1f(m_level1, l1);
	glUniform1f(m_level2, l2);
	glUniform1f(m_level3, l3);
	glUniform1f(m_level4, l4);
	glUniform1f(m_level5, l5);
}
