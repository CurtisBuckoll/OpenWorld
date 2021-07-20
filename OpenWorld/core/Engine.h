#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "graphics/gl/ShaderProgram.h"

// -----------------------------------------------------------------
//
namespace ow
{
namespace core
{

// =======================================================================
//
enum class state { RUN, EXIT };

// =======================================================================
//
class Engine
{

public:

   // -----------------------------------------------------------------
   //
   Engine() = delete;
   ~Engine() = default;
   Engine( const Engine& ) = delete;
   Engine( Engine&& ) = delete;
   Engine operator=( const Engine& ) = delete;
   Engine operator=( Engine&& ) = delete;

   // -----------------------------------------------------------------
   //
   Engine( int win_width,
           int win_height );

   // -----------------------------------------------------------------
   //
   void init();

   // -----------------------------------------------------------------
   //
   //void run();

   // -----------------------------------------------------------------
   //
   void loop();

   // -----------------------------------------------------------------
   //
   SDL_Window* window() { return window_; }

private:

   // -----------------------------------------------------------------
   //
   void process_system_exit();

   // -----------------------------------------------------------------
   //
   SDL_Window* window_;
   int         width_;
   int         height_;
   state       state_;
};

}
}
