#pragma once

#include <GL/glew.h>

#include <memory>
#include <string>

namespace ow
{
namespace core
{
class Texture
{
public:
   Texture() = delete;
   Texture( const Texture& ) = delete;
   Texture& operator=( const Texture& ) = delete;

   //Texture( const std::shared_ptr<uint8_t>& data );
   Texture( const std::string& filepath, bool genMips = true );
   ~Texture();

   void bind( uint32_t slot );

   void update();

private:

   void init( uint8_t* data, bool genMips );

   GLuint id_ = 0u;
   GLsizei width_ = 0;
   GLsizei height_ = 0;
  
};

}
}
