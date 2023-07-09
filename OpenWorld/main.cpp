// including windows.h messes up some stuff with min/max macros, so
// define NOMINMAX to subvert that. glew may need some things from
// windows.h
// we can probably move these includes to better places though later, so
// make sure this is adressed better if it can be
#define NOMINMAX
#include <Windows.h>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"
#include "core/graphics/gl/Buffer.h"
#include "core/graphics/gl/Framebuffer.h"
#include "model/Model.h"
#include "model/Skybox.h"

#include "core/Camera3D.h"
#include "core/InputState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "io/Logging.h"

struct RenderConstants
{
   glm::mat4 model_;
   glm::mat4 view_;
   glm::mat4 proj_;
   glm::vec3 viewPos_;
};

struct Vertex
{
   float pos[3];
   float normal[3];
   float uv[2];
};

static constexpr ow::AttribFormat inputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
   {1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, normal )},
   {2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, uv )}
};

static constexpr ow::AttribFormat skyboxInputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
};

// TODO: read from file
// parameters
static const int32_t kWinWidth = 1000;
static const int32_t kWinHeight = 600;

// =======================================================================
//
int main( int argc, char** argv )
{
   OW_LOG( INFO, "Lauching Open World" );
   OW_LOG( INFO, "initializing window with window resolution: %dx%d", kWinWidth, kWinHeight );

   ow::Engine engine( kWinWidth, kWinHeight );
   engine.init();

   ow::ShaderProgram shaderProgram( "simpleShader_vs",
                                    "simpleShader_fs",
                                    inputLayout,
                                    sizeof( inputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram reflectShaderProgram( "simpleShader_vs",
                                           "reflective_fs",
                                           inputLayout,
                                           sizeof( inputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram skyboxShaderProgram( "skyboxShader_vs",
                                          "skyboxShader_fs",
                                          skyboxInputLayout,
                                          sizeof( skyboxInputLayout ) / sizeof( ow::AttribFormat ) );

   auto sampler = std::make_shared<ow::Sampler>( ow::SamplerType::LinFilterLinMips );

   // test making a framebuffer
   auto fb = std::make_shared<ow::Framebuffer>( kWinWidth, kWinHeight );

   ow::Model backpackModel = ow::Model( "assets\\test\\backpack\\backpack.obj" );

   auto skybox = std::make_shared<ow::Skybox>( "assets\\test\\brudslojan_skybox\\" );

   OW_LOG_INFO( "starting main loop" );

   RenderConstants renderConstants;
   renderConstants.proj_ = glm::perspective( glm::radians( 60.0f ), (float)kWinWidth / (float)kWinHeight, 0.1f, 100.0f );
   renderConstants.view_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -3.0f ) );
   renderConstants.model_ = glm::mat4( 1.0f );

   ow::Buffer constantBuffer( ow::BufferUsage::UniformBuffer, sizeof( RenderConstants ), 0, nullptr );

   // input state and camera test
   InputState inputState;
   Camera3D cam;
   cam.init( glm::vec3( 0.0f, 0.0f, 3.0f ) );

   bool running = true;
   while( running )
   {
      //fb->bind();

      running = inputState.pollForEvents();
      cam.processInput( inputState );

      renderConstants.view_ = cam.viewMatrix();
      renderConstants.viewPos_ = cam.viewPosition();

      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.5f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glEnable( GL_DEPTH_TEST );

      // 1. reflective rendering for fun:

      //reflectShaderProgram.use();
      //glEnable( GL_DEPTH_TEST );
      //glDepthFunc( GL_LESS );
      //constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      //constantBuffer.bind( 0 );
      //cubemapTexture->bind( 2, sampler );

      //backpackModel.draw();
      //reflectShaderProgram.unuse();


      // 2. usual rendering of backpack:

      shaderProgram.use();
      glDepthFunc( GL_LESS );
      constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      constantBuffer.bind( 0 );
      backpackModel.draw();
      shaderProgram.unuse();

      // 3. render skybox last for perf

      skyboxShaderProgram.use();
      constantBuffer.bind( 0 );
      skybox->draw();
      skyboxShaderProgram.unuse();

      SDL_GL_SwapWindow( engine.window() );
   }

   //engine.loop();

   return 0;
}