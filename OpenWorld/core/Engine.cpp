#include "Engine.h"

#include "io/Logging.h"


// =======================================================================
//
namespace ow
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

   // initGL
   glViewport( 0, 0, width_, height_ );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
   SDL_GLContext glContext = SDL_GL_CreateContext( window_ );
   if( glContext == nullptr )
   {
      OW_LOG( ERRO, "SDL_GL Context failed to create" );
      exit( 0 );
   }

   // init GLEW
   glewExperimental = true;
   if( glewInit() != GLEW_OK )
   {
      OW_LOG( ERRO, "Could not initalize GLEW" );
      exit( 0 );
   }
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
