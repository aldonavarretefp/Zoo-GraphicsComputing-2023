
#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace SpotLight{
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum SpotLight_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 6.0f;
	const GLfloat SENSITIVTY = 0.25f;
	const GLfloat ZOOM = 45.0f;

	// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
	class SpotLight
	{
	public:
		// Constructor with vectors
		SpotLight(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
		{
			this->position = position;
			this->worldUp = up;
			this->yaw = yaw;
			this->pitch = pitch;
			this->updateSpotLightVectors();
		}



		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessMovement(SpotLight_Movement direction, GLfloat deltaTime)
		{
			GLfloat velocity = this->movementSpeed * deltaTime;

			if (direction == FORWARD)
			{
				this->position += this->front * velocity;
			}

			if (direction == BACKWARD)
			{
				this->position -= this->front * velocity;
			}

			if (direction == LEFT)
			{
				this->position -= this->right * velocity;
			}

			if (direction == RIGHT)
			{
				this->position += this->right * velocity;
			}
		}


		void ProcessDirection(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) {
			this->yaw += xOffset;
			this->pitch += yOffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (this->pitch > 89.0f)
				{
					this->pitch = 89.0f;
				}

				if (this->pitch < -89.0f)
				{
					this->pitch = -89.0f;
				}
			}

			// Update Front, Right and Up Vectors using the updated Eular angles
			this->updateSpotLightVectors();
		}

		glm::vec3 GetPosition()
		{
			return this->position;
		}

		glm::vec3 GetFront()
		{
			return this->front;
		}

	private:
		// SpotLight Attributes
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;

		// Eular Angles
		GLfloat yaw;
		GLfloat pitch;

		// SpotLight options
		GLfloat movementSpeed;
		GLfloat mouseSensitivity;
		GLfloat zoom;

		// Calculates the front vector from the SpotLight's (updated) Eular Angles
		void updateSpotLightVectors()
		{
			// Calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			front.y = sin(glm::radians(this->pitch));
			front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			this->front = glm::normalize(front);
			// Also re-calculate the Right and Up vector
			this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			this->up = glm::normalize(glm::cross(this->right, this->front));
		}
	};
}
