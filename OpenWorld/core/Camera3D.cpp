#include "Camera3D.h"

#include <SDL/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// =======================================================================
//
void Camera3D::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );
	front.y = sin( glm::radians( pitch_ ) );
	front.z = sin( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );
	front_ = glm::normalize( front );
	right_ = glm::normalize( glm::cross( front_, up_ ) );
}

// =======================================================================
//
void Camera3D::init( const glm::vec3& position, 
							const glm::vec3& up )
{
	position_ = position;
	up_ = up;
	updateCameraVectors();
}

// =======================================================================
//
glm::mat4 Camera3D::viewMatrix() const
{
	return glm::lookAt( position_, position_ + front_, up_ );
}

// =======================================================================
//
void Camera3D::processInput( InputState& inputState )
{
	processMouse( inputState.mouseState() );
	processKeyboard( inputState.keys() );
}

// =======================================================================
//
void Camera3D::processKeyboard( const bool* keys )
{
	static float deltaTime = 0.0f;
	static float lastFrame = 0.0f;

	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float velocity = movementSpeed_ * deltaTime;
	if( keys[SDLK_w] )
	{
		position_ += front_ * velocity;
	}
	if( keys[SDLK_s] )
	{
			position_ -= front_ * velocity;
	}
	if( keys[SDLK_a] )
	{
		position_ -= right_ * velocity;
	}
	if( keys[SDLK_d] )
	{
		position_ += right_ * velocity;
	}
}

// =======================================================================
//
void Camera3D::processMouse( const MouseState* pos )
{
	static float lastX = 0;
	static float lastY = 0;
	float xoffset;
	float yoffset;

	if( firstMouse_ )
	{
		lastX = pos->x_;
		lastY = pos->y_;
		firstMouse_ = false;
		return;
	}

	xoffset = pos->x_ - lastX;
	yoffset = lastY - pos->y_;
	lastX = pos->x_;
	lastY = pos->y_;
	yaw_ += xoffset * mouseSensitivity_;
	pitch_ += yoffset * mouseSensitivity_;

	if( pitch_ > 89.0f )
	{
		pitch_ = 89.0f;
	}
	if( pitch_ < -89.0f )
	{
		pitch_ = -89.0f;
	}

	updateCameraVectors();
}
