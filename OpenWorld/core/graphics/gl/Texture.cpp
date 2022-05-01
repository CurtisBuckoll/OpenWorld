#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "3rdparty/io/stb_image.h"

#include "io/Logging.h"

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

//void Texture::bind()
//{
//   glActiveTexture( GL_TEXTURE0 );
//   glBindTexture( GL_TEXTURE_2D, id_ );
//}

void Texture::bind( uint32_t slot, const std::shared_ptr<Sampler>& sampler )
{
   glActiveTexture( GL_TEXTURE0 + slot );
   glBindTexture( GL_TEXTURE_2D, id_ );

   // sampler
   // TODO: we could make these samplers Global in some Engine.cpp, or something like
   // that. Then we pass the sampler enum in to the bind() here instead of the actual sampler.
   // not sure yet.. maybe we want to move all bind calls to Engine.cpp (or Graphics.cpp)?
   sampler->bind( slot );

   //glBindSampler( slot, samplerId_ );
}

void Texture::unbind()
{
   // not sure we really need this, so maybe do nothing for now
}

void Texture::update()
{
   // TODO
}

void Texture::init( uint8_t* data, bool genMips )
{
   if( !data )
   {
      OW_LOG( ERRO, "(LOG) ERROR: Null texture data in Texture::init()" );
      exit( 0 );
   }

   glGenTextures( 1, &id_ );
   glBindTexture( GL_TEXTURE_2D, id_ );

   // fix texture params for now
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   auto format = numChannels_ == 3 ? GL_RGB : GL_RGBA; // TODO: make more robust

   glTexImage2D( GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data );

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
