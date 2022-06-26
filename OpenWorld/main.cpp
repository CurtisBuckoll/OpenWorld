#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"
#include "core/graphics/gl/Buffer.h"
#include "core/graphics/gl/Framebuffer.h"
#include "model/Mesh.h"

#include "core/Camera3D.h"
#include "core/InputState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "io/Logging.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f ),
    glm::vec3( 2.0f,  5.0f, -15.0f ),
    glm::vec3( -1.5f, -2.2f, -2.5f ),
    glm::vec3( -3.8f, -2.0f, -12.3f ),
    glm::vec3( 2.4f, -0.4f, -3.5f ),
    glm::vec3( -1.7f,  3.0f, -7.5f ),
    glm::vec3( 1.3f, -2.0f, -2.5f ),
    glm::vec3( 1.5f,  2.0f, -2.5f ),
    glm::vec3( 1.5f,  0.2f, -1.5f ),
    glm::vec3( -1.3f,  1.0f, -1.5f )
};

struct RenderConstants
{
   glm::mat4 model_;
   glm::mat4 view_;
   glm::mat4 proj_;
};

struct Vertex
{
   float pos[3];
   float uv[2];
};

static constexpr ow::core::AttribFormat inputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
   {1, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, uv )}
};

//static constexpr ow::core::AttribFormat inputLayout[] = {
//   {0, 3, GL_FLOAT, GL_FALSE, 0},
//   {1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, colour )},
//   {2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, uv )}
//};

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

   ow::core::Engine engine( kWinWidth, kWinHeight );
   engine.init();

   // -------------------------------------
   // begin texture code - test textures
   std::string texturePath = ow::core::workingDir() + "assets\\test\\wall.jpg";
   ow::core::Texture testTexture( texturePath, true );
   texturePath = ow::core::workingDir() + "assets\\test\\container.jpg";
   ow::core::Texture testTexture2( texturePath, true );

   ow::Buffer vboCube( ow::BufferUsage::VertexBuffer, sizeof( vertices ), sizeof( Vertex ), vertices );

   ow::core::ShaderProgram shaderProgram( "simpleShader_vs",
                                          "simpleShader_fs",
                                          inputLayout,
                                          sizeof( inputLayout ) / sizeof( ow::core::AttribFormat ) );

   auto sampler = std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips );

   // test making a framebuffer
   auto fb = std::make_shared<ow::Framebuffer>( kWinWidth, kWinHeight );

   // test of mesh, basically let's see if we can replace one of the above with a mesh implementation
   // might want to undo this later, or see if we can save this unit test code in a better way

   std::vector<ow::Vertex> owTestVertices = {
      // positions          // colors           // texture coords
      { glm::vec3{0.0f,  0.0f, -0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f} }, // top right
      { glm::vec3{0.0f, -1.0f, -0.5f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{1.0f, 0.0f} }, // bottom right
      { glm::vec3{-1.0f, -1.0f, -0.5f},   glm::vec3{0.0f, 0.0f, 1.0f},   glm::vec2{0.0f, 0.0f}}, // bottom left
      { glm::vec3{-1.0f,  0.0f, -0.5f},   glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec2{0.0f, 1.0f}}  // top left
   };

   std::vector<uint32_t> owTestIndices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
   };

   std::vector<std::shared_ptr<ow::core::Texture>> dummyTex;

   auto testMesh = std::make_shared<ow::Mesh>( owTestVertices, owTestIndices, dummyTex, dummyTex );

   OW_LOG( INFO, "starting main loop" );

   RenderConstants renderConstants;
   renderConstants.proj_ = glm::perspective( glm::radians( 45.0f ), (float)kWinWidth / (float)kWinHeight, 0.1f, 100.0f );
   renderConstants.view_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -3.0f ) );

   ow::Buffer constantBuffer( ow::BufferUsage::UniformBuffer, sizeof( RenderConstants ), 0, nullptr );

   // input state and camera test
   InputState inputState;
   Camera3D cam;
   cam.Init( glm::vec3( 0.0f, 0.0f, 3.0f ) );

   bool running = true;
   while( running )
   {
      //fb->bind();

      running = inputState.pollForEvents();
      cam.ProcessKeyboard( inputState.keys() );

      renderConstants.view_ = cam.GetViewMatrix();

      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.5f, 1.0f );

      glEnable( GL_DEPTH_TEST );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      shaderProgram.use();

      testTexture.bind( 0, sampler );
      testTexture2.bind( 1, sampler );

      vboCube.bind();

      for( unsigned int i = 0; i < 10; i++ )
      {
         glm::mat4 model = glm::mat4( 1.0f );
         model = glm::translate( model, cubePositions[i] );
         float angle = 20.0f * i;
         renderConstants.model_ = glm::rotate( model, glm::radians( angle ), glm::vec3( 1.0f, 0.3f, 0.5f ) );

         constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
         constantBuffer.bind( 0 );

         glDrawArrays( GL_TRIANGLES, 0, 36 );
      }

      //glDrawArrays( GL_TRIANGLES, 0, 36 );

      //testMesh->draw();

      //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
      //glDrawArrays( GL_TRIANGLES, 0, 3 );

      SDL_GL_SwapWindow( engine.window() );

      shaderProgram.unuse();
   }

   //engine.loop();

   return 0;
}