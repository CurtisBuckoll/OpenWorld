#pragma once

#include <glm/glm.hpp>

#include "InputState.h"

// =======================================================================
//
class Camera3D
{
public:
	// -----------------------------------------------------------------
	//
	Camera3D() 
		: front_( glm::vec3( 0.0f, 0.0f, -1.0f ) )
	{}

	// -----------------------------------------------------------------
	//
	virtual ~Camera3D() {}

	// -----------------------------------------------------------------
	//
	void init( const glm::vec3& position, 
				  const glm::vec3& up = glm::vec3( 0.0f, 1.0f, 0.0f ) );

	// -----------------------------------------------------------------
	//
	glm::mat4 viewMatrix() const;

	// -----------------------------------------------------------------
	//
	glm::vec3 viewPosition() const { return position_; }

	// -----------------------------------------------------------------
	//
	void processInput( InputState& inputState );

private:
	// -----------------------------------------------------------------
	//
	void updateCameraVectors();

	// -----------------------------------------------------------------
	//
	void processKeyboard( const bool* keys );

	// -----------------------------------------------------------------
	//
	void processMouse( const MouseState* pos );

	// -----------------------------------------------------------------
	//
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;

	// -90 yaw by default so we look down -ve z axis
	float yaw_ = -90.0f;
	float pitch_ = 0.0f;

	const float movementSpeed_ = 0.0025f;
	const float mouseSensitivity_ = 0.25f;

	bool firstMouse_ = true;
};
