#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"

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

// parameters
static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;

int main( int argc, char** argv )
{

   ow::core::Engine engine( kWIN_WIDTH, kWIN_HEIGHT );
   engine.init();

   // -------------------------------------
   // initGL
   glViewport( 0, 0, kWIN_WIDTH, kWIN_HEIGHT );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
   SDL_GLContext glContext = SDL_GL_CreateContext( engine.window() );
   if( glContext == nullptr )
   {
      std::cout << "SDL_GL Context failed to create";
   }
   /*	Initialize GLEW  */
   glewExperimental = true;
   if( glewInit() != GLEW_OK )
   {
      std::cout << "Could not initalize GLEW";
      exit( 0 );
   }
   // -------------------------------------
   // end initGL

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

   // shaders
   unsigned int vertexShader;
   vertexShader = glCreateShader( GL_VERTEX_SHADER );
   glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
   glCompileShader( vertexShader );

   int  success;
   char infoLog[512];
   glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
   if( !success )
   {
      glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   unsigned int fragmentShader;
   fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
   glCompileShader( fragmentShader );
   glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
   if( !success )
   {
      glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   unsigned int shaderProgram;
   shaderProgram = glCreateProgram();
   glAttachShader( shaderProgram, vertexShader );
   glAttachShader( shaderProgram, fragmentShader );
   glLinkProgram( shaderProgram );
   glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
   if( !success )
   {
      glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
   }

   glDeleteShader( vertexShader );
   glDeleteShader( fragmentShader );

   //glEnable(GL_BLEND);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glEnable(GL_DEPTH_TEST);

   while( true )
   {
      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


      glUseProgram( shaderProgram );
      glBindVertexArray( VAO );
      glDrawArrays( GL_TRIANGLES, 0, 3 );

      SDL_GL_SwapWindow( engine.window() );
   }



   // -------------------------------------
   // end tri code

   //engine.loop();

   return 0;
}