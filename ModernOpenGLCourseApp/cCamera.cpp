#include "cCamera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

Camera::~Camera()
{
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	// Parameters of glm::lookAt:
	//	1 - The position of our camera or eye
	//	2 - The position of something we want to look towards
	//	3 - The local up vector relative to the camera
	return glm::lookAt(position, position + front, up);	// You can manipulate this to give a 3rd person effect. If we had a player we want to follow, set argument 1 to the position around our player, and argument 2 to the position of the player.
}

void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));

	up = glm::normalize(glm::cross(right, front));
}
