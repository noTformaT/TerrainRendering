#include "DirectionalShadowMapRenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

bool DirectionalShadowMapRenderSystem::Init()
{
	if (!RenderSystem::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/shadow_map.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/shadow_map.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	m_modelLoc = GetUniformLocation("model");
	m_lightTransform = GetUniformLocation("directionalLightSpaceTransform");

	return true;
}

void DirectionalShadowMapRenderSystem::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(m_lightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void DirectionalShadowMapRenderSystem::SetModelTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(*lTransform));
}
