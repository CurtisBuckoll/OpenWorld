#pragma once

#include <stdint.h>

#include <array>

static constexpr uint32_t kKeymapSize = 2048;

class InputState
{
public:

	InputState();
	~InputState() {};

	// returns false if should exit
	bool pollForEvents();

	bool* keys();

private:

	std::array<bool, kKeymapSize> keymap_;
};