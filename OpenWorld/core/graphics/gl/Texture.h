#pragma once

#include <GL/glew.h>

#include <memory>
#include <string>
#include <vector>

#include "Sampler.h"

namespace ow
{

// =======================================================================
//
class Texture
{
public:
   // -----------------------------------------------------------------
   //
   Texture() = delete;
   Texture( const Texture& ) = delete;
   Texture& operator=( const Texture& ) = delete;
   Texture( Texture&& ) = delete;
   Texture& operator=( Texture&& ) = delete;

   // -----------------------------------------------------------------
   //
   Texture( const std::string& filepath, 
            bool genMips = true );
   Texture( uint32_t width, 
            uint32_t height, 
            bool isDepth, 
            bool genMips = false );
   Texture( const std::vector<std::string>& cubemapFiles );
   ~Texture();

   // -----------------------------------------------------------------
   //
   void bind( uint32_t slot, const std::shared_ptr<Sampler>& sampler );

   // -----------------------------------------------------------------
   //
   void unbind();

   // -----------------------------------------------------------------
   //
   void update();

   // -----------------------------------------------------------------
   //
   inline GLuint id() { return id_; }

private:

   // -----------------------------------------------------------------
   //
   void init( uint8_t* data, 
              bool genMips, 
              bool isDepthTex );

   // -----------------------------------------------------------------
   //
   GLuint  id_          = 0u;
   GLsizei width_       = 0;
   GLsizei height_      = 0;
   GLsizei numChannels_ = 0;
   bool isCubemap_      = false;

   // sampler
   // TODO: this seemed to work, but we probably want to create a Sampler class with instantions somewhere
   // and then specify those when we call bind() on the texture. That way we can share/reuse samplers and not
   // have textures/samplers 1:1.
   // this would allow us to easily use something like: layout(binding = x) uniform sampler2D ourTexture
   // -> maybe init all the samplers in Engine.cpp?
   // see: https://stackoverflow.com/questions/19085956/what-is-the-difference-between-gltexparameter-and-glsamplerparameter
   //GLuint samplerId_    = 0u;
};

}
