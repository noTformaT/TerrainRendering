#include "ShadowBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void ShadowBox::Init(glm::mat4 lightView, Camera* camera)
{
	lightViewMatrix = lightView;
	cam = camera;
	calculateWidthAndHeight();
}

ShadowBox::ShadowBox()
{

}

ShadowBox::~ShadowBox()
{
}

void ShadowBox::calculateWidthAndHeight()
{
	farWidth = SHADOW_DISTANCE * glm::tan(glm::radians(FOV));
	nearWidth = NEAR_PLANE * glm::tan(glm::radians(FOV));

	farHeight = farWidth / getAspectRatio();
	nearWidth = nearWidth / getAspectRatio();
}

float ShadowBox::getAspectRatio()
{
	return 1024.0f / 768;
}

void ShadowBox::update()
{
	glm::mat4 rotation = calculateCameraRotationMatrix();

	glm::vec4 t1 = rotation * FORWARD;
	glm::vec3 forwardVector = t1.xyz;

	glm::vec3 toFar = forwardVector * SHADOW_DISTANCE;
	glm::vec3 toNear = forwardVector * NEAR_PLANE;
	glm::vec3 centerNear = toNear + cam->GetPosition();
	glm::vec3 centerFar = toFar + cam->GetPosition();

	std::vector<glm::vec4> points = calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);

	bool first = true;
	for (glm::vec4& point : points)
	{
		if (first)
		{
			minX = point.x;
			maxX = point.x;
			minY = point.y;
			maxY = point.y;
			minZ = point.z;
			maxZ = point.z;
			first = false;
			continue;
		}

		if (point.x > maxX) {
			maxX = point.x;
		}
		else if (point.x < minX) {
			minX = point.x;
		}
		if (point.y > maxY) {
			maxY = point.y;
		}
		else if (point.y < minY) {
			minY = point.y;
		}
		if (point.z > maxZ) {
			maxZ = point.z;
		}
		else if (point.z < minZ) {
			minZ = point.z;
		}
	}
	maxZ += OFFSET;
}

glm::mat4 ShadowBox::calculateCameraRotationMatrix()
{
	return glm::mat4();
}

std::vector<glm::vec4> ShadowBox::calculateFrustumVertices(glm::mat4 rotation, glm::vec3 forwardVector, glm::vec3 centerNear, glm::vec3 centerFar)
{
	glm::vec4 t1 = rotation * UP;

	glm::vec3 upVector = t1.xyz;
	glm::vec3 rightVector = glm::cross(forwardVector, upVector);
	glm::vec3 downVector = upVector * -1.0f;
	glm::vec3 leftVector = rightVector * -1.0f;
	glm::vec3 farTop = centerFar + glm::vec3(upVector.x * farHeight, upVector.y * farHeight, upVector.z * farHeight);
	glm::vec3 farBottom = centerFar + glm::vec3(downVector.x * farHeight, downVector.y * farHeight, downVector.z * farHeight);
	glm::vec3 nearTop = centerNear + glm::vec3(upVector.x * nearHeight, upVector.y * nearHeight, upVector.z * nearHeight);
	glm::vec3 nearBottom = centerNear + glm::vec3(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);

	std::vector<glm::vec4> points;
	points.push_back(calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth));
	points.push_back(calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth));
	return points;
}

glm::vec4 ShadowBox::calculateLightSpaceFrustumCorner(glm::vec3 startPoint, glm::vec3 direction, float width)
{
	glm::vec3 point = startPoint + glm::vec3(direction.x * width, direction.y * width, direction.z * width);
	glm::vec4 point4f = glm::vec4(point.x, point.y, point.z, 1.0f);
	point4f = lightViewMatrix * point4f;
	return point4f;
}
