#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"
#include "core/graphics/gl/Buffer.h"

#include "io/Logging.h"

// temporary
struct Vertex
{
   float pos[3];
   float colour[3];
   float uv[2];
};

Vertex verticesQuad[] = {
   // positions          // colors           // texture coords
   { 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f}, // top right
   { 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f}, // bottom right
   {-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f}, // bottom left
   {-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f}  // top left
};

Vertex verticesQuad2[] = {
   // positions          // colors           // texture coords
   { 0.0f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f}, // top right
   { 0.0f, -1.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f}, // bottom right
   {-1.0f, -1.0f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f}, // bottom left
   {-1.0f,  0.0f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f}  // top left
};

//Vertex verticesQuad[] = {
//    // positions         // texture coords
//   { 0.5f,  0.5f, 0.0f,  1.0f, 1.0f}, // top right
//   { 0.5f, -0.5f, 0.0f,  1.0f, 0.0f}, // bottom right
//   {-0.5f, -0.5f, 0.0f,  0.0f, 0.0f}, // bottom left
//   {-0.5f,  0.5f, 0.0f,  0.0f, 1.0f}  // top left
//};

uint32_t indicesQuad[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};


static constexpr ow::core::AttribFormat inputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
   {1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, colour )},
   {2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, uv )}
};

// TODO: read from file
// parameters
static const int kWIN_WIDTH = 1000;
static const int kWIN_HEIGHT = 600;

// =======================================================================
//
int main( int argc, char** argv )
{
  OW_LOG( INFO, "A MSG" );
  OW_LOG( WARN, "A MSG AND ARG: %d", 1 );
  OW_LOG( ERRO, "A MSG AND TWO ARG: %d, %d", 1, 2 );

   ow::core::Engine engine( kWIN_WIDTH, kWIN_HEIGHT );
   engine.init();

   // -------------------------------------
   // begin texture code
   // quick test for texture
   std::string texturePath = ow::core::workingDir() + "assets\\test\\wall.jpg";
   ow::core::Texture testTexture( texturePath, true );
   texturePath = ow::core::workingDir() + "assets\\test\\container.jpg";
   ow::core::Texture testTexture2( texturePath, true );

   Vertex vertices[] = {
       // positions          // texture coords
      {-0.5f, -0.5f, 0.0f,   0.0f, 0.0f},   // bottom left
      { 0.5f, -0.5f, 0.0f,   1.0f, 0.0f},   // bottom right
      { 0.0f,  0.5f, 0.0f,   0.5f, 1.0f}    // top
   };

   uint32_t indices[] = { 0, 1, 2 };

   Vertex vertices2[] = {
      // positions          // texture coords
      {-1.0f, -1.0f, -0.5f,   0.5f, 0.0f},   // bottom left
      { 0.0f, -1.0f, -0.5f,   1.0f, 0.0f},   // bottom right
      {-0.5f,  0.0f, -0.5f,   0.5f, 0.5f}    // top
   };

   ow::core::Buffer VBO1( ow::core::BufferUsage::VertexBufferObject, sizeof( verticesQuad ), sizeof( Vertex ), verticesQuad );
   ow::core::Buffer VBO2( ow::core::BufferUsage::VertexBufferObject, sizeof( verticesQuad2 ), sizeof( Vertex ), verticesQuad2 );
   ow::core::Buffer EBO( ow::core::BufferUsage::ElementBufferObject, sizeof( indicesQuad ), sizeof( uint32_t ), indicesQuad );
   ow::core::ShaderProgram shaderProgram( "simpleShader_vs",
                                          "simpleShader_fs",
                                          inputLayout,
                                          sizeof( inputLayout ) / sizeof( ow::core::AttribFormat ) );

   auto sampler = std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips );

   OW_LOG( INFO, "Starting main loop" );

   int tmpCount = 0;
   while( true )
   {
      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      shaderProgram.use();

      testTexture.bind( 0, sampler );
      testTexture2.bind( 1, sampler );

      if(( tmpCount % 3000) == 0 )
      {
         VBO1.bind( 0 );
      }
      else if( (tmpCount % 3000) == 3000 / 2 )
      {
         VBO2.bind( 0 );
      }
      EBO.bind();

      glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
      //glDrawArrays( GL_TRIANGLES, 0, 3 );

      SDL_GL_SwapWindow( engine.window() );

      EBO.unbind();
      VBO1.unbind();

      shaderProgram.unuse();
      ++tmpCount;
   }

   //engine.loop();

   return 0;
}