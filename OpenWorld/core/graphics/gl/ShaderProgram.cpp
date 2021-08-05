#include "ShaderProgram.h"

#include <filesystem>
#include <iostream>
#include <string>

#include "../../Platform.h"
#include "../../../io/FileReader.h" // TODO: clean up include paths

namespace
{
static const std::string shaderFolder = "shaders";
static const std::string shaderExt = ".glsl";
}

namespace ow
{
namespace core
{

// =======================================================================
//
ShaderProgram::ShaderProgram( const std::string& vsName, const std::string& fsName )
{
   std::filesystem::path shaderDir( workingDir() );
   shaderDir.append( shaderFolder ).append( vsName ).concat( shaderExt );

   FileReader reader;
   auto vsSource = reader.read( shaderDir.string(), false );

   // shaders
   unsigned int vsId;
   vsId = glCreateShader( GL_VERTEX_SHADER );
   auto vsData = reinterpret_cast<const GLchar*>(vsSource);
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

   shaderDir.replace_filename( fsName ).replace_extension( shaderExt );
   auto fsSource = reader.read( shaderDir.string(), false );

   unsigned int fsId;
   fsId = glCreateShader( GL_FRAGMENT_SHADER );
   auto fsData = reinterpret_cast<const GLchar*>(fsSource);
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
