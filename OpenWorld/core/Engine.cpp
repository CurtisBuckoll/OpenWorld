#include "Engine.h"


// =======================================================================
//
namespace ow
{
namespace core
{

// =======================================================================
//
Engine::Engine( int win_width,
                int win_height )
   : window_( nullptr )
   , width_( win_width )
   , height_( win_height )
   , state_( state::RUN )
{

}

// =======================================================================
//
void Engine::init()
{
   SDL_Init( SDL_INIT_EVERYTHING );
   window_ = SDL_CreateWindow( "OpenWorld",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width_,
                               height_,
                               SDL_WINDOW_OPENGL );
}

//// =======================================================================
////
//void Engine::run()
//{
//    init();
//    loop();
//}

// =======================================================================
//
void Engine::loop()
{
   while( state_ != state::EXIT )
   {
      process_system_exit();
   }
}

// =======================================================================
//
void Engine::process_system_exit()
{
   SDL_Event e;
   while( SDL_PollEvent( &e ) )
   {
      if( e.type == SDL_QUIT )
      {
         state_ = state::EXIT;
      }
   }
}

}
}
