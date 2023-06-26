#include "ShadowMapViewRenderSystem.h"

ShadowMapViewRenderSystem::ShadowMapViewRenderSystem()
{
}

bool ShadowMapViewRenderSystem::Init()
{
	if (!RenderSystem::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/shadow_view.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/shadow_view.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	Enable();
	depthTextureLoc = GetUniformLocation("depthTexture");

	glUniform1i(depthTextureLoc, 0);
}


