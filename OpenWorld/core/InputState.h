#pragma once

#include <stdint.h>

#include <array>

static constexpr uint32_t kKeymapSize = 2048;

struct MouseState
{
	int32_t x_ = 0;
	int32_t y_ = 0;
	bool leftButtonHold_ = false;
	bool rightButtonHold_ = false;
};

class InputState
{
public:

	InputState();
	~InputState() {};

	// returns false if should exit
	bool pollForEvents();

	bool* keys() { return keymap_.data(); }

	MouseState* mouseState() { return &mouseState_; }

private:

	std::array<bool, kKeymapSize> keymap_;
	MouseState mouseState_;
};