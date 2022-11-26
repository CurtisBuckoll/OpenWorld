#include "ShaderProgram.h"

#include <filesystem>
#include <string>

#include "core/Platform.h"
#include "io/FileReader.h"
#include "io/Logging.h"

namespace
{
static constexpr std::string_view shaderFolder = "shaders";
static constexpr std::string_view shaderExt    = ".glsl";
}

namespace ow
{
namespace core
{

// =======================================================================
//
ShaderProgram::ShaderProgram( const std::string& vsName,
                              const std::string& fsName,
                              const AttribFormat* attribs,
                              uint32_t numAttribs )
{
   std::filesystem::path shaderDir( workingDir() );
   shaderDir.append( shaderFolder ).append( vsName ).concat( shaderExt );

   // vertex shader
   FileReader reader;
   auto vsSource = reader.read( shaderDir.string(), false );

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
      OW_LOG( ERRO, "Vertex shader compile failure:\n%s", errorLog );
   }

   // fragment shader
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
      OW_LOG( ERRO, "Fragment shader compile failure:\n%s", errorLog );
   }

   // TODO: geometry shader

   // shader program
   id_ = glCreateProgram();
   glAttachShader( id_, vsId );
   glAttachShader( id_, fsId );
   glLinkProgram( id_ );
   glGetProgramiv( id_, GL_LINK_STATUS, &status );
   if( !status )
   {
      glGetProgramInfoLog( id_, 512, nullptr, errorLog );
      OW_LOG( ERRO, "Linking shader program failure:\n%s", errorLog );
   }

   glDeleteShader( vsId );
   glDeleteShader( fsId );

   // create the vao
   glGenVertexArrays( 1, &vao_ );
   glBindVertexArray( vao_ );

   // vertix attribute layout
   for( uint32_t i = 0; i < numAttribs; ++i )
   {
      glEnableVertexAttribArray( i );
      glVertexAttribFormat( attribs[i].idx_,
                            attribs[i].numValues_,
                            attribs[i].type_,
                            attribs[i].normalized_,
                            attribs[i].offset_ );
      glVertexAttribBinding( i, 0 );
   }

   glBindVertexArray( 0 );
}

// =======================================================================
//
ShaderProgram::~ShaderProgram()
{
   glDeleteShader( id_ );
   id_ = 0;
   glDeleteVertexArrays( 1, &vao_ );
   vao_ = 0;
}

// =======================================================================
//
void ShaderProgram::use()
{
   glUseProgram( id_ );
   glBindVertexArray( vao_ );

   // TODO: let's bind the samplers here
}

// =======================================================================
//
void ShaderProgram::unuse()
{
   glUseProgram( 0 );
   glBindVertexArray( 0 );
}

}
}
