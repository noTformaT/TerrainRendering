#pragma once

#include <glm/glm.hpp>

struct LightingData
{
	glm::vec3 SunDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 SunColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float SunIntencity = 1.0f;
	float SunDiffuse = 0.1f;
	bool LitMaterial = true;
};