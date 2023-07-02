#include "SkyBoxRenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

SkyBoxRenderSystem::SkyBoxRenderSystem()
{
}

SkyBoxRenderSystem::~SkyBoxRenderSystem()
{
}

bool SkyBoxRenderSystem::Init()
{
	if (!RenderSystem::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/skybox.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/skybox.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	m_projectionLoc = GetUniformLocation("projection");
	m_viewLoc = GetUniformLocation("view");

	return true;
}

void SkyBoxRenderSystem::UpdateViewProjection(glm::mat4& view, glm::mat4& projection)
{
	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
