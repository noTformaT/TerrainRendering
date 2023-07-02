#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"
#include "ShadowBox.h"

struct LightingData
{
	glm::vec3 SunDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 SunColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float SunIntencity = 1.0f;
	float SunDiffuse = 0.1f;
	bool LitMaterial = true;

	glm::mat4 lightProj;

	ShadowMap shadowMap;

	ShadowBox shadowBox;

	glm::mat4 lightViewMatrix;

	void Init()
	{
		lightViewMatrix = glm::mat4(1.0f);

		shadowMap.Init(2048, 2048);

		float near_plane = 0.0f, far_plane = 300.5f;
		lightProj = glm::ortho(-128.0f, 128.0f, -128.0f, 128.0f, near_plane, far_plane);

		//lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 2000.0f);
	}

	glm::mat4 CalculateLightTransform(glm::vec3& center)
	{
		glm::mat4 lightView = glm::lookAt(center,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		return lightProj * lightView;

		//return lightProj * glm::lookAt(center, center + SunDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}
};