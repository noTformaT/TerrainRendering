#pragma once

#include "RenderSystem.h"

#include <glm/glm.hpp>

class ShadowMapViewRenderSystem : public RenderSystem
{
public:
	ShadowMapViewRenderSystem();

	virtual bool Init() override;

private:
	GLint depthTextureLoc = 0;
};