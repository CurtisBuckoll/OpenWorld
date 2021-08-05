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

   void bind(); // TODO: remove

   void update();

private:

   void init( uint8_t* data, bool genMips );

   GLuint id_           = 0u;
   GLsizei width_       = 0;
   GLsizei height_      = 0;
   GLsizei numChannels_ = 0;

   // sampler
   // TODO: this seemed to work, but we probably want to create a Sampler class with instantions somewhere
   // and then specify those when we call bind() on the texture. That way we can share/reuse samplers and not
   // have textures/samplers 1:1.
   // this would allow us to easily use something like: layout(binding = x) uniform sampler2D ourTexture
   // see: https://stackoverflow.com/questions/19085956/what-is-the-difference-between-gltexparameter-and-glsamplerparameter
   //GLuint samplerId_    = 0u;
};

}
}
