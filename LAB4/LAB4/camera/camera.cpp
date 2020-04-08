#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

deng::Camera::Camera(glm::vec3 position)
{
	this->_position = position;

	//calc pitch and yaw angles
	this->_pitch = 0.0f;
	this->_yaw = 0.0f;
	this->_up = glm::vec3(0, 1, 0);

	this->_front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	this->_front.y = sin(glm::radians(this->_pitch));
	this->_front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_yaw));
	this->_front = glm::normalize(this->_front);
}

glm::mat4 deng::Camera::GetViewMatrix()
{
	return glm::lookAt(this->_position,this->_position + this->_front, this->_up);
}

void deng::Camera::MoveForward(GLfloat distance)
{
	this->_position.x += this->_front.x * distance;
	this->_position.y += this->_front.y * distance;
	this->_position.z += this->_front.z * distance;
}

void deng::Camera::MoveBack(GLfloat distance)
{
	this->_position.x -= this->_front.x * distance;
	this->_position.y -= this->_front.y * distance;
	this->_position.z -= this->_front.z * distance;
}

void deng::Camera::MoveRight(GLfloat distance)
{
	GLfloat yaw = this->_yaw + 90.0f;
	glm::vec3 camera_right;
	camera_right.x = cos(glm::radians(this->_pitch)) * cos(glm::radians(yaw));
	camera_right.y = sin(glm::radians(this->_pitch));
	camera_right.z = cos(glm::radians(this->_pitch)) * sin(glm::radians(yaw));

	this->_position.x += camera_right.x * distance;
	this->_position.y += camera_right.y * distance;
	this->_position.z += camera_right.z * distance;
}

void deng::Camera::MoveLeft(GLfloat distance)
{
	GLfloat yaw = this->_yaw + 90.0f;
	glm::vec3 camera_right;
	camera_right.x = cos(glm::radians(this->_pitch)) * cos(glm::radians(yaw));
	camera_right.y = sin(glm::radians(this->_pitch));
	camera_right.z = cos(glm::radians(this->_pitch)) * sin(glm::radians(yaw));

	this->_position.x -= camera_right.x * distance;
	this->_position.y -= camera_right.y * distance;
	this->_position.z -= camera_right.z * distance;
}

void deng::Camera::Turn(GLfloat dpitch, GLfloat dyaw)
{
	this->_pitch += dpitch;
	if (_pitch >= 90)
		_pitch = 90;
	if (_pitch <= -90)
		_pitch = -90;

	this->_yaw += dyaw;

	this->_front.x = cos(glm::radians(this->_pitch)) * cos(glm::radians(this->_yaw));
	this->_front.y = sin(glm::radians(this->_pitch));
	this->_front.z = cos(glm::radians(this->_pitch)) * sin(glm::radians(this->_yaw));
	this->_front = glm::normalize(this->_front);
}
