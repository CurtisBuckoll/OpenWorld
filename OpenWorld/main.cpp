#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"

// temp -------------------------------------------------
const char* vertexShaderSource = "#version 330 core\n"
   "layout (location = 0) in vec3 aPos;\n"
   "void main()\n"
   "{\n"
   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
   "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
   "out vec4 FragColor;\n"
   "void main()\n"
   "{\n"
   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
   "}\0";
// -----------------------------------------------------

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


   float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
   };


   // -------------------------------------
   // end texture code

   // -------------------------------------
   // begin tri code
   // vbo
   float vertices[] = { -0.5f, -0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f,
                         0.0f,  0.5f, 0.0f };

   unsigned int VAO;
   glGenVertexArrays( 1, &VAO );
   glBindVertexArray( VAO );

   unsigned int VBO;
   glGenBuffers( 1, &VBO );
   glBindBuffer( GL_ARRAY_BUFFER, VBO );
   glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
   glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
   glEnableVertexAttribArray( 0 );

   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray( 0 );

   ow::core::ShaderProgram shaderProgram( vertexShaderSource, fragmentShaderSource );

   while( true )
   {
      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      shaderProgram.use();

      glBindVertexArray( VAO );
      glDrawArrays( GL_TRIANGLES, 0, 3 );

      SDL_GL_SwapWindow( engine.window() );

      shaderProgram.unuse();
   }

   // -------------------------------------
   // end tri code

   //engine.loop();

   return 0;
}