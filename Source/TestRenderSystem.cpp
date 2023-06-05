#include "TestRenderSystem.h"

bool TestRenderSystem::Init()
{
	if (!RenderSystem::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Shaders/test.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/test.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	return true;
}
