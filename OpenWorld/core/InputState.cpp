#include "InputState.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <SDL/SDL.h>

#include <cstring>


InputState::InputState()
{
	std::memset( keymap_.data(), 0, sizeof(keymap_));
}

bool InputState::pollForEvents()
{
	auto buttonState = SDL_GetMouseState( &mouseState_.x_, &mouseState_.y_ );
	mouseState_.leftButtonHold_ = buttonState & SDL_BUTTON_LMASK;
	mouseState_.rightButtonHold_ = buttonState & SDL_BUTTON_RMASK;

	SDL_Event evnt;
	while( SDL_PollEvent( &evnt ) )
	{
		// forward input to imgui
		ImGui_ImplSDL2_ProcessEvent( &evnt );

		// process for our own app
		switch( evnt.type )
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			keymap_[evnt.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			keymap_[evnt.key.keysym.sym] = false;
			break;
		}
	}
	return true;
}
