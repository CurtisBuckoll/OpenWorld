#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"

struct Vertex
{
   float pos[3];
   float uv[2];
};

static constexpr ow::core::AttribFormat inputLayout[2] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
   {1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv)}
};

// TODO: read from file
// parameters
static const int kWIN_WIDTH = 1000;
static const int kWIN_HEIGHT = 600;

int main( int argc, char** argv )
{
   ow::core::Engine engine( kWIN_WIDTH, kWIN_HEIGHT );
   engine.init();

   // -------------------------------------
   // begin texture code
   // quick test for texture
   std::string texturePath = ow::core::workingDir() + "assets\\test\\wall.jpg";
   ow::core::Texture testTexture( texturePath );


   //float texCoords[] = {
   // 0.0f, 0.0f,  // lower-left corner  
   // 1.0f, 0.0f,  // lower-right corner
   // 0.5f, 1.0f   // top-center corner
   //};


   // -------------------------------------
   // end texture code

   // -------------------------------------
   // begin tri code
   // vbo
   //float vertices[] = { -0.5f, -0.5f, 0.0f,
   //                      0.5f, -0.5f, 0.0f,
   //                      0.0f,  0.5f, 0.0f };

   //float vertices[] = {
   //   // positions          // texture coords
   //    0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
   //    0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
   //   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
   //   -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
   //};

   float vertices[] = {
      // positions          // texture coords
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
       0.0f,  0.5f, 0.0f,   0.5f, 1.0f    // top
   };

   float vertices2[] = {
      // positions          // texture coords
      -1.0f, -1.0f, -0.5f,   0.0f, 0.0f,   // bottom left
       0.0f, -1.0f, -0.5f,   1.0f, 0.0f,   // bottom right
      -0.5f,  0.0f, -0.5f,   0.5f, 0.5f    // top
   };

   // TODO: see to seperate buffer from layout: https://www.khronos.org/opengl/wiki/Vertex_Specification#Separate_attribute_format
   //unsigned int VAO;
   //glGenVertexArrays( 1, &VAO );
   //glBindVertexArray( VAO );

   //unsigned int VBO;
   //glGenBuffers( 1, &VBO );
   //glBindBuffer( GL_ARRAY_BUFFER, VBO );
   //glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

   //// position
   //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)0 );
   //glEnableVertexAttribArray( 0 );

   //// texture uv
   //glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)(3 * sizeof( float )) );
   //glEnableVertexAttribArray( 1 );

   //glBindBuffer( GL_ARRAY_BUFFER, 0 );
   //glBindVertexArray( 0 );

   // ---------
   unsigned int VBO1;
   glGenBuffers( 1, &VBO1 );
   glBindBuffer( GL_ARRAY_BUFFER, VBO1 );
   glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );

   unsigned int VBO2;
   glGenBuffers( 1, &VBO2 );
   glBindBuffer( GL_ARRAY_BUFFER, VBO2 );
   glBufferData( GL_ARRAY_BUFFER, sizeof( vertices2 ), vertices2, GL_STATIC_DRAW );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );

   //unsigned int VAO;
   //glGenVertexArrays( 1, &VAO );
   //glBindVertexArray( VAO );

   ////glBindVertexBuffer( 0, VBO, 0, 5 * sizeof(float) );
   ////glVertexBindingDivisor( 0, 0 );

   //glEnableVertexAttribArray( 0 );
   //glVertexAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, 0 );
   //glVertexAttribBinding( 0, 0 );
   //glEnableVertexAttribArray( 1 );
   //glVertexAttribFormat( 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float) );
   //glVertexAttribBinding( 1, 0 );

   ////glBindBuffer( GL_ARRAY_BUFFER, 0 );
   //glBindVertexArray( 0 );

   // ---------

   ow::core::ShaderProgram shaderProgram( "simpleShader_vs",
                                          "simpleShader_fs",
                                          inputLayout,
                                          sizeof( inputLayout ) / sizeof( ow::core::AttribFormat ) );

   int tmpCount = 0;
   while( true )
   {
      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      shaderProgram.use();

      testTexture.bind();

      static unsigned int vboToUse = VBO1;
      if(( tmpCount % 1000) == 0 )
      {
         vboToUse = VBO1;
      }
      else if( (tmpCount % 1000) == 1000 / 2 )
      {
         vboToUse = VBO2;
      }
      glBindVertexBuffer( 0, vboToUse, 0, 5 * sizeof( float ) );
      glVertexBindingDivisor( 0, 0 ); // TODO: do we need this??


      glDrawArrays( GL_TRIANGLES, 0, 3 );

      SDL_GL_SwapWindow( engine.window() );

      shaderProgram.unuse();
      ++tmpCount;
   }

   // -------------------------------------
   // end tri code

   //engine.loop();

   return 0;
}