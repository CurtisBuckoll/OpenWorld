#include "ShaderProgram.h"

#include <iostream>
#include <string>

namespace ow
{
namespace core
{

// =======================================================================
//
ShaderProgram::ShaderProgram( const std::string& vsSource, const std::string& fsSource )
{
   // shaders
   unsigned int vsId;
   vsId = glCreateShader( GL_VERTEX_SHADER );
   auto vsData = static_cast<const GLchar*>(vsSource.c_str());
   glShaderSource( vsId, 1, &vsData, nullptr );
   glCompileShader( vsId );

   int  status;
   char errorLog[512];
   glGetShaderiv( vsId, GL_COMPILE_STATUS, &status );
   if( !status )
   {
      glGetShaderInfoLog( vsId, 512, nullptr, errorLog );
      std::cout << "Vertex shader compile failure\n" << errorLog << std::endl;
   }

   unsigned int fsId;
   fsId = glCreateShader( GL_FRAGMENT_SHADER );
   auto fsData = static_cast<const GLchar*>(fsSource.c_str());
   glShaderSource( fsId, 1, &fsData, nullptr );
   glCompileShader( fsId );
   glGetShaderiv( fsId, GL_COMPILE_STATUS, &status );
   if( !status )
   {
      glGetShaderInfoLog( fsId, 512, nullptr, errorLog );
      std::cout << "Fragment shader compile failure\n" << errorLog << std::endl;
   }

   id_ = glCreateProgram();
   glAttachShader( id_, vsId );
   glAttachShader( id_, fsId );
   glLinkProgram( id_ );
   glGetProgramiv( id_, GL_LINK_STATUS, &status );
   if( !status )
   {
      glGetProgramInfoLog( id_, 512, nullptr, errorLog );
      std::cout << "Linking shader program failure\n" << errorLog << std::endl;
   }

   glDeleteShader( vsId );
   glDeleteShader( fsId );
}

// =======================================================================
//
ShaderProgram::~ShaderProgram()
{
   glDeleteShader( id_);
}


// =======================================================================
//
void ShaderProgram::use()
{
   glUseProgram( id_ );
}

// =======================================================================
//
void ShaderProgram::unuse()
{
   glUseProgram( 0 );
}

}
}
