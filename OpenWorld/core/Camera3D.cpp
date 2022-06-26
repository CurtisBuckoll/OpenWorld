#include "Camera3D.h"
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>


// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera3D::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos( glm::radians( _yaw ) ) * cos( glm::radians( _pitch ) );
	front.y = sin( glm::radians( _pitch ) );
	front.z = sin( glm::radians( _yaw ) ) * cos( glm::radians( _pitch ) );
	_front = glm::normalize( front );

	// Re-calculate the Right and Up vector
	_right = glm::normalize( glm::cross( _front, _worldUp ) );
	_up = glm::normalize( glm::cross( _right, _front ) );
}


// Default Constructor
Camera3D::Camera3D() :
	_front( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
	_movementSpeed( SPEED ),
	_mouseSensitivity( SENSITIVTY ),
	_zoom( ZOOM )
{
	// Empty
}


// Initialise camera variables
void Camera3D::Init( CAMERA_MODE mode, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch )
{
	_mode = mode;
	_position = position;
	_worldUp = up;
	_yaw = yaw;
	_pitch = pitch;
	updateCameraVectors();
}


// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera3D::GetViewMatrix()
{
	return glm::lookAt( _position, _position + _front, _up );
}


// Processes input received from any keyboard-like input system.
void Camera3D::ProcessKeyboard( bool* keys )
{
	static float deltaTime = 0.0f;
	static float lastFrame = 0.0f;

	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat velocity = _movementSpeed * deltaTime;
	if( keys[SDLK_w] )
	{
		if( _mode == CAMERA_MODE::WALK )
		{
			_position.x += _front.x * velocity;
			_position.z += _front.z * velocity;
		}
		else
		{
			_position += _front * velocity;
		}
	}
	if( keys[SDLK_s] )
	{
		if( _mode == CAMERA_MODE::WALK )
		{
			_position.x -= _front.x * velocity;
			_position.z -= _front.z * velocity;
		}
		else
		{
			_position -= _front * velocity;
		}
	}
	if( keys[SDLK_a] )
	{
		_position -= _right * velocity;
	}
	if( keys[SDLK_d] )
	{
		_position += _right * velocity;
	}

	static bool toggle = true;
	if( keys[SDLK_m] && toggle )
	{
		if( _mode == CAMERA_MODE::FLY )
		{
			_mode = CAMERA_MODE::WALK;
		}
		else
		{
			_mode = CAMERA_MODE::FLY;
		}
		toggle = false;
	}
	if( !keys[SDLK_m] )
	{
		toggle = true;
	}
}


// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera3D::ProcessMouseMovement( glm::vec2 pos )
{
	static GLfloat lastX = 0;
	static GLfloat lastY = 0;
	GLfloat xoffset;
	GLfloat yoffset;

	static bool firstMouse = true;
	if( firstMouse )
	{
		lastX = pos.x;
		lastY = pos.y;
		firstMouse = false;
		return;
	}

	xoffset = pos.x - lastX;
	yoffset = lastY - pos.y;
	lastX = pos.x;
	lastY = pos.y;
	xoffset *= _mouseSensitivity;
	yoffset *= _mouseSensitivity;
	_yaw += xoffset;
	_pitch += yoffset;

	if( _pitch > 89.0f )
	{
		_pitch = 89.0f;
	}
	if( _pitch < -89.0f )
	{
		_pitch = -89.0f;
	}

	updateCameraVectors();
}

glm::vec3 Camera3D::getPosition()
{
	return _position;
}