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
   Framebuffer( uint32_t width, uint32_t height, bool depthOnly = false );

   // -----------------------------------------------------------------
   //
   ~Framebuffer();

   // -----------------------------------------------------------------
   //
   void bind();

   // -----------------------------------------------------------------
   //
   void unbind();

   // -----------------------------------------------------------------
   // // TODO: extract this method and any others to a base class for most openGL objects to extend *********************
   GLuint id() { return id_; }

   GLuint colourTexId() { return colourTex_->id(); }

   GLuint depthTexId() { return depthTex_->id(); }

//private:

   // -----------------------------------------------------------------
   //
   GLuint id_ = 0;

   std::shared_ptr<ow::Texture> colourTex_;
   std::shared_ptr<ow::Texture> depthTex_;
};
}
