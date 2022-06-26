#pragma once


#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.001f;
const GLfloat SENSITIVTY = 0.35f;
const GLfloat ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera3D
{
private:
	// Camera Attributes
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
	// Eular Angles
	GLfloat _yaw;
	GLfloat _pitch;
	// Camera options
	GLfloat _movementSpeed;
	GLfloat _mouseSensitivity;
	GLfloat _zoom;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();

public:
	Camera3D();

	// Initialise
	void Init(glm::vec3 position = glm::vec3( 0.0f, 0.0f, 1.5f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), GLfloat yaw = YAW, GLfloat pitch = PITCH );

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Processes input received from any keyboard-like input system.
	void ProcessKeyboard( bool* keys );

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement( glm::vec2 pos );

	glm::vec3 getPosition();
};