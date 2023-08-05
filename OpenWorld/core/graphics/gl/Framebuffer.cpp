#pragma once

#include "Framebuffer.h"

#include <GL/glew.h>

#include <stdint.h>

#include "io/Logging.h"
#include "core/graphics/gl/Texture.h"


namespace ow
{

// =======================================================================
//
Framebuffer::Framebuffer( uint32_t width, uint32_t height, bool depthOnly /* = false */ )
{
   glGenFramebuffers( 1, &id_ );
   glBindFramebuffer( GL_FRAMEBUFFER, id_ );

   depthTex_ = std::make_shared<ow::Texture>( width, height, true );
   glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex_->id(), 0 );

   if( depthOnly )
   {
      // need to specify no colour buffer used
      glDrawBuffer( GL_NONE );
      glReadBuffer( GL_NONE );
   }
   else
   {
      colourTex_ = std::make_shared<ow::Texture>( width, height, false );
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTex_->id(), 0 );
   }


   if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
   {
      OW_LOG( ERRO, "Framebuffer failed to initialize completely" );
   }
   else // delete this
   {
      OW_LOG( INFO, "Framebuffer okay!" );
   }

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// =======================================================================
//
Framebuffer::~Framebuffer()
{
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   glDeleteFramebuffers( 1, &id_ );
}

// =======================================================================
//
void Framebuffer::bind()
{
   glBindFramebuffer( GL_FRAMEBUFFER, id_ );
}

// =======================================================================
//
void Framebuffer::unbind()
{
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
}
