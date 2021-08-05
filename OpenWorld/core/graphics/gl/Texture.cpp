#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "3rdparty/io/stb_image.h"

#include <iostream>

namespace ow
{
namespace core
{

// TODO: create texture cache by ptr lookup

// might want w/h/c, maybe this isn't needed for now
//Texture::Texture( const std::shared_ptr<uint8_t>& data )
//   : id_(0)
//{
//   init( data.get() );
//}

Texture::Texture( const std::string& filepath, bool genMips )
{
   // tell stb_image.h to flip texture
   stbi_set_flip_vertically_on_load( true );

   uint8_t* data = stbi_load( filepath.c_str(), &width_, &height_, &numChannels_, 0 );
   init( data, genMips );
   stbi_image_free( data );
}

Texture::~Texture()
{
   glDeleteTextures( 1, &id_ );
   id_ = 0;

   // sampler
   //glDeleteSamplers( 1, &samplerId_ );
   //samplerId_ = 0;
}

void Texture::bind()
{
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, id_ );
}

void Texture::bind( uint32_t slot )
{
   glActiveTexture( GL_TEXTURE0 + slot );
   glBindTexture( GL_TEXTURE_2D, id_ );

   // sampler
   //glBindSampler( slot, samplerId_ );
}

void Texture::update()
{

}

void Texture::init( uint8_t* data, bool genMips )
{
   if( !data )
   {
      // TODO: error properly using errors class/logs
      std::cout << "(LOG) ERROR: Null texture data in Texture::init()" << std::endl;
      exit( 0 );
   }

   glGenTextures( 1, &id_ );
   glBindTexture( GL_TEXTURE_2D, id_ );

   // fix texture params for now
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data );

   if( genMips )
   {
      glGenerateMipmap( GL_TEXTURE_2D );
   }

   glBindTexture( GL_TEXTURE_2D, 0 );

   // sampler
   //glGenSamplers( 1, &samplerId_ );
   //glSamplerParameter*(...);
}

}
}
