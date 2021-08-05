#pragma once

#include <GL/glew.h>

#include <string>

// see here to seperate vbo from vao format:
// https://www.khronos.org/opengl/wiki/Vertex_Specification#Separate_attribute_format

namespace ow
{
namespace core
{

struct AttribFormat
{
   GLuint    idx_;
   GLint     numValues_;
   GLenum    type_;
   GLboolean normalized_;
   GLuint    offset_;
};

// =======================================================================
//
class ShaderProgram
{
public:
   // -----------------------------------------------------------------
   //
   ShaderProgram() = delete;
   ShaderProgram( const ShaderProgram& ) = delete;
   ShaderProgram& operator=( const ShaderProgram& ) = delete;

   // -----------------------------------------------------------------
   //
   ShaderProgram( const std::string& vsName,
                  const std::string& fsName,
                  const AttribFormat* attribs,
                  uint32_t numAttribs );

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

   // -----------------------------------------------------------------
   //
   GLuint id_;
   GLuint vao_;
};

}
}
