
#pragma once

#include <vector>

#define GLEW_STATIC

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

enum   Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


const GLfloat YAWS = -90.f;
const GLfloat PITCH = 0.0f;
const GLfloat  SPEED = 6.0f;
const GLfloat  SENSITIVITY = 0.25f;
const GLfloat   ZOOM = 45.0f;

class Camera
{
public:
	Camera
	(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
		GLfloat yaw = YAWS,
	GLfloat pitch = PITCH
		):front(glm::vec3 (0.0f,0.0f,-1.0f)),movmentSpd(SPEED),
		mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->UpdateCameraVector();


	}

	Camera
	(GLfloat posX,
		GLfloat posY,
		GLfloat posZ,
		GLfloat upX,
		GLfloat upY,
		GLfloat upZ,
		GLfloat yaw,
		GLfloat pitch) 
		:front(glm::vec3(0.0f, 0.0f, -1.0f))
		, movmentSpd(SPEED),mouseSensitivity(SENSITIVITY),zoom(ZOOM)
	{
		this->position = glm::vec3(posX,posY,posZ);
		this->worldUp = glm::vec3(upX,upY,upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->UpdateCameraVector();

	}

	glm::mat4 GetViewmatrix()
	{
		return glm::lookAt(this->position,
							this->position+this->front,
			this->up);
}


	void ProcessInputFunction
	(Camera_Movement direction,GLfloat deltaTime)
	{
		GLfloat velocity = this->movmentSpd *deltaTime;

		if(FORWARD == direction)
		{
			this->position += this->front * velocity;

		}


		if (BACKWARD == direction)
		{
			this->position -= this->front * velocity;

		}
		if (LEFT == direction)
		{
			this->position -= this->right* velocity;

		}

		if (RIGHT == direction)
		{
			this->position += this->right * velocity;

		}


	}



	void ProcessMouseMovmentFunction
	(GLfloat xoffset,GLfloat yOffset,GLboolean constainPitchboolean = true)
	{
		xoffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xoffset;
		this->pitch += yOffset;

		if (constainPitchboolean)
		{
			if(this->pitch> 89.0f)
			{

				this->pitch = 89.0f;
			}
		
			if (this->pitch <-89.0f)
			{
				this->pitch = -89.0f;
			}
		
		
		}
		this->UpdateCameraVector();
	}

	void ProcessMouseScrollFunction
	(GLfloat yoffset)
	{
	

	}

	GLfloat GetZoomFunction()
	{
		return this->zoom;
	}

	glm::vec3 GetPosition()
	{
		return this->position;
	}

	glm::vec3 GetFrontFunction()
	{
		return this->front;
	}

private:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movmentSpd;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	void UpdateCameraVector()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw))
			* cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));

		front.z = sin(glm::radians(this->yaw))* 
			cos(glm::radians(this->pitch));

		this->front = glm::normalize(front);

		this->right = 
			glm::normalize(glm::cross(this->front,this->worldUp));

		this->up = glm::normalize
		(glm::cross(this->right,this->front));



	}


};

