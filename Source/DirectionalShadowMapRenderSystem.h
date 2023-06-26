#pragma once

#include "RenderSystem.h"

#include <glm/glm.hpp>

class DirectionalShadowMapRenderSystem : public RenderSystem
{
public:
	virtual bool Init() override;

	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetModelTransform(glm::mat4* lTransform);
private:
	GLuint m_modelLoc = 0;
	GLuint m_lightTransform = 0;
};