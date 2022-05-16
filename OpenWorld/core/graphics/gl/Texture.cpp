#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "3rdparty/io/stb_image.h"

#include "core/Platform.h"
#include "io/Logging.h"


// TODO: create texture cache by ptr lookup


namespace ow
{
namespace core
{

// =======================================================================
//
Texture::Texture( const std::string& filepath, 
                  bool genMips /* = true */ )
{
   // tell stb_image.h to flip texture
   stbi_set_flip_vertically_on_load( true );

   uint8_t* data = stbi_load( filepath.c_str(), &width_, &height_, &numChannels_, 0 );

   if( !data )
   {
      OW_LOG( ERRO, "Bad texture data read in Texture::Texture() from filepath: %s", filepath.c_str() );
   }

   init( data, genMips, false );
   stbi_image_free( data );
}

// =======================================================================
//
Texture::Texture( uint32_t width,
                  uint32_t height,
                  bool isDepth,
                  bool genMips /* = false */ )
   : width_( width )
   , height_( height )
   , numChannels_(3)
{
   // for now, default at 3 channels as generally we will use this to create
   // framebuffer texture attachments. in case of depth, numChannels is not
   // used at all
   init( nullptr, genMips, isDepth );
}

// =======================================================================
//
Texture::~Texture()
{
   glDeleteTextures( 1, &id_ );
   id_ = 0;

   // sampler
   //glDeleteSamplers( 1, &samplerId_ );
   //samplerId_ = 0;
}

// =======================================================================
//
//void Texture::bind()
//{
//   glActiveTexture( GL_TEXTURE0 );
//   glBindTexture( GL_TEXTURE_2D, id_ );
//}

// =======================================================================
//
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

// =======================================================================
//
void Texture::unbind()
{
   // not sure we really need this, so maybe do nothing for now
}

// =======================================================================
//
void Texture::update()
{
   // TODO
}

// =======================================================================
//
void Texture::init( uint8_t* data, bool genMips, bool isDepthTex)
{
   glGenTextures( 1, &id_ );
   glBindTexture( GL_TEXTURE_2D, id_ );

   if( isDepthTex )
   {
      // we are creating a depth texture and currently no data should be 
      // supplied here
      OW_ASSERT(data == nullptr);
      glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr );
      OW_LOG( INFO, "created depth attachment (delete log)");
   }
   else
   {
      // fix texture params for now
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      // TODO: make more robust, and later will maybe have to expand to 
      // include more options
      OW_ASSERT( numChannels_ == 3 || numChannels_ == 4 );
      auto format = numChannels_ == 3 ? GL_RGB : GL_RGBA;

      glTexImage2D( GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data );

      if (genMips)
      {
         glGenerateMipmap(GL_TEXTURE_2D);
      }
   }

   glBindTexture( GL_TEXTURE_2D, 0 );

   // sampler
   //glGenSamplers( 1, &samplerId_ );
   //glSamplerParameter*(...);
}

}
}
