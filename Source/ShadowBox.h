#pragma once

#define GLM_SWIZZLE 
#include <glm/glm.hpp>

#include "Camera.h"
#include "Util.h"
#include <vector>

class ShadowBox
{
public:
	void Init(glm::mat4 lightView, Camera* camera);
	ShadowBox();
	~ShadowBox();

private:
	float OFFSET = 10.0f;
	glm::vec4 UP = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 FORWARD = glm::vec4(0.0f, 1.0f, -1.0f, 0.0f);
	float SHADOW_DISTANCE = 100.0f;

	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	float farHeight;
	float farWidth;
	float nearHeight;
	float nearWidth;

	glm::mat4 lightViewMatrix;
	Camera* cam = nullptr;

	void calculateWidthAndHeight();
	float getAspectRatio();
public:
	void update();
	glm::mat4 calculateCameraRotationMatrix();
	std::vector<glm::vec4> calculateFrustumVertices(glm::mat4 rotation, glm::vec3 forwardVector, glm::vec3 centerNear, glm::vec3 centerFar);
	glm::vec4 calculateLightSpaceFrustumCorner(glm::vec3 startPoint, glm::vec3 direction, float width);
	
};