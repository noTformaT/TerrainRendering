#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 initPosition, glm::vec3 initUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed, GLfloat initTurnSpeed)
{
	position = initPosition;
	worldUp = initUp;
	yaw = initYaw;
	pitch = initPitch;
	front = glm::vec3(.0f, .0f, -1.0f);

	movementSpeed = initMoveSpeed;
	turnSpeed = initTurnSpeed;

	UpdateCamera();
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	if (!keys)
	{
		return;
	}

	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_Q])
	{
		position -= worldUp * velocity;
	}

	if (keys[GLFW_KEY_E])
	{
		position += worldUp * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else
	{
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	UpdateCamera();
}

glm::mat4 Camera::CalculateVewMatrix()
{
	glm::vec3 look = position + front;

	//printf("pos x:%f y:%f z:%f, look x:%f y:%f z:%f \n", position.x, position.y, position.z, look.x, look.y, look.z);

	

	glm::mat4 view = glm::lookAt(position, look, up);
	return projection * view;
}

void Camera::SetProjection(glm::mat4 proj)
{
	projection = proj;
}

Camera::~Camera()
{

}

void Camera::UpdateCamera()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
