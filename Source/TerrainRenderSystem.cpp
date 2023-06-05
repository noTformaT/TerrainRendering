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
		return false;
	}

	return true;
}

void TerrainRenderSystem::SetVP(const glm::mat4& vp)
{
	glUniformMatrix4fv(m_VPLoc, 1, GL_FALSE, glm::value_ptr(vp));
}
