#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>


namespace ow
{
namespace core
{

// =======================================================================
//
class ShaderProgram
{
public:
   // -----------------------------------------------------------------
   //
   ShaderProgram() = delete;
   ShaderProgram( const ShaderProgram& ) = delete;
   ShaderProgram operator=( const ShaderProgram& ) = delete;

   // -----------------------------------------------------------------
   //
   ShaderProgram( const std::string& vsSource, const std::string& fsSource );

   // -----------------------------------------------------------------
   //
   ~ShaderProgram();

   // -----------------------------------------------------------------
   //
   GLuint id() { return id_; }

   // -----------------------------------------------------------------
   //
   void use();

   // -----------------------------------------------------------------
   //
   void unuse();

private:
   GLuint id_;
};

}
}
