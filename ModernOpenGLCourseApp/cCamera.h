#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 CalculateViewMatrix();

private:
	glm::vec3 position,
		front,
		up,
		right,
		worldUp;

	GLfloat yaw,
		pitch,
		movementSpeed,
		turnSpeed;

	void Update();
};

