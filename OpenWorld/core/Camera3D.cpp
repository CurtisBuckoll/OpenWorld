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
void Camera3D::init( glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch )
{
	_position = position;
	_worldUp = up;
	_yaw = yaw;
	_pitch = pitch;
	updateCameraVectors();
}


// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera3D::viewMatrix() const
{
	return glm::lookAt( _position, _position + _front, _up );
}


void Camera3D::processInput( InputState& inputState )
{
	processMouse( inputState.mouseState() );
	processKeyboard( inputState.keys() );
}


// Processes input received from any keyboard-like input system.
void Camera3D::processKeyboard( const bool* keys )
{
	static float deltaTime = 0.0f;
	static float lastFrame = 0.0f;

	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat velocity = _movementSpeed * deltaTime;
	if( keys[SDLK_w] )
	{
		_position += _front * velocity;
	}
	if( keys[SDLK_s] )
	{
			_position -= _front * velocity;
	}
	if( keys[SDLK_a] )
	{
		_position -= _right * velocity;
	}
	if( keys[SDLK_d] )
	{
		_position += _right * velocity;
	}
}


// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera3D::processMouse( const MouseState* pos )
{
	static GLfloat lastX = 0;
	static GLfloat lastY = 0;
	GLfloat xoffset;
	GLfloat yoffset;

	static bool firstMouse = true;
	if( firstMouse )
	{
		lastX = pos->x_;
		lastY = pos->y_;
		firstMouse = false;
		return;
	}

	xoffset = pos->x_ - lastX;
	yoffset = lastY - pos->y_;
	lastX = pos->x_;
	lastY = pos->y_;
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

glm::vec3 Camera3D::position()
{
	return _position;
}