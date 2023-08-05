#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "3rdparty/io/stb_image.h"

#include "core/Platform.h"
#include "io/Logging.h"


namespace ow
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
Texture::Texture( const std::vector<std::string>& cubemapFiles )
   : isCubemap_( true )
{
   // we could maybe merge this to share init code below, but leaving for now since i's
   // not super important and maybe more readable this way
   //const std::string dir = ow::core::workingDir() + "assets\\test\\skybox\\";

   OW_ASSERT( cubemapFiles.size() == 6 );
   glGenTextures( 1, &id_ );
   glBindTexture( GL_TEXTURE_CUBE_MAP, id_ );

   for( uint32_t i = 0; i < cubemapFiles.size(); i++ )
   {
      // tell stb_image.h to flip texture
      stbi_set_flip_vertically_on_load( false );

      unsigned char* data = stbi_load( (cubemapFiles[i]).c_str(), &width_, &height_, &numChannels_, 0 );
      if( data )
      {
         glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                       0, 
                       GL_RGB, 
                       width_, 
                       height_, 
                       0, 
                       GL_RGB, GL_UNSIGNED_BYTE, 
                       data );
      }
      else
      {
         OW_LOG_WARN( "Texture::Texture (cubemap) - failed to load file %s", cubemapFiles[i] );
      }
      stbi_image_free( data );
   }
   glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
   glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
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
   glBindTexture( isCubemap_ ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, id_ );

   // sampler
   // TODO: we could make these samplers Global in some Engine.cpp, or something like
   // that. Then we pass the sampler enum in to the bind() here instead of the actual sampler.
   // not sure yet.. maybe we want to move all bind calls to Engine.cpp (or Graphics.cpp)?
   //
   // BIG NOTE: disabling this still works, especially for enabling cube map textures. but
   // we will need a longer term solution for this, so next thing we do should probably be
   // to address above TODO
   // NOTE: this is probably because texture sampling paramaters take effect in place of
   // sampler parameters if no sampler is bound. this is assuming the sampler paramaters
   // override the texture parameters. we should verify if that is indeed the case
   //sampler->bind( slot );

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
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
      OW_LOG( INFO, "created depth attachment (delete log)");
   }
   else
   {
      // fix texture params for now
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

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
}

}
