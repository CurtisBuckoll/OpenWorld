#include "InputState.h"

#include <SDL/SDL.h>

#include <cstring>


InputState::InputState()
{
	std::memset( keymap_.data(), 0, sizeof(keymap_));
}

bool InputState::pollForEvents()
{
	SDL_Event event;
	while( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			keymap_[event.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			keymap_[event.key.keysym.sym] = false;
			break;
		}
	}
	return true;
}

bool* InputState::keys()
{
	return keymap_.data();
}
