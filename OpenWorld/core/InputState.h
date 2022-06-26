#pragma once

#include <stdint.h>

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

	bool keymap[kKeymapSize];
};