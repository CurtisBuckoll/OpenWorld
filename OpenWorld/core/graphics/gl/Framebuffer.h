#pragma once

#include <GL/glew.h>

#include <stdint.h>

#include "io/Logging.h"
#include "core/graphics/gl/Texture.h"


namespace ow
{

// =======================================================================
//
class Framebuffer
{
public:

   // -----------------------------------------------------------------
   //
   Framebuffer() = delete;
   Framebuffer( const Framebuffer& ) = delete;
   Framebuffer& operator=( const Framebuffer& ) = delete;
   Framebuffer( Framebuffer&& ) = delete;
   Framebuffer& operator=( Framebuffer&& ) = delete;

   // -----------------------------------------------------------------
   //
   Framebuffer( uint32_t width, uint32_t height )
   {
      glGenFramebuffers(1, &id_);
      glBindFramebuffer(GL_FRAMEBUFFER, id_);

      auto tex = std::make_shared<ow::Texture>(width, height, false);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id(), 0);

      auto depthTex = std::make_shared<ow::Texture>(width, height, true);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->id(), 0);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
      {
         OW_LOG( ERRO, "Framebuffer failed to initialize completely" );
      }
      else // delete this
      {
         OW_LOG( INFO, "Framebuffer okay!" );
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   // -----------------------------------------------------------------
   //
   ~Framebuffer()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDeleteFramebuffers(1, &id_);
   }

   // -----------------------------------------------------------------
   //
   void bind()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, id_);
   }

   // -----------------------------------------------------------------
   //
   void unbind()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   // -----------------------------------------------------------------
   // // TODO: extract this method and any others to a base class for most openGL objects to extend *********************
   inline GLuint id() { return id_; }

private:

   // -----------------------------------------------------------------
   //
   GLuint id_ = 0;
};
}
