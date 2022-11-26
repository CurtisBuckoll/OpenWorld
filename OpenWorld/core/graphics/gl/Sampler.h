#pragma once

#include <GL/glew.h>

#include <stdint.h>

namespace ow
{

enum class SamplerType
{
   LinFilterLinMips,
   COUNT
};

// =======================================================================
//
class Sampler
{
public:

   // -----------------------------------------------------------------
   //
   Sampler() = delete;
   Sampler( const Sampler& ) = delete;
   Sampler& operator=( const Sampler& ) = delete;
   Sampler( const Sampler&& ) = delete;
   Sampler& operator=( const Sampler&& ) = delete;

   // -----------------------------------------------------------------
   //
   Sampler( SamplerType type )
   {
      glGenSamplers( 1, &id_ );

      glSamplerParameteri( id_, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glSamplerParameteri( id_, GL_TEXTURE_WRAP_T, GL_REPEAT );

      switch( type )
      {
      case SamplerType::LinFilterLinMips:
         glSamplerParameteri( id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
         glSamplerParameteri( id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         break;
      default:
         // raise some error
         break;
      }
   }

   // -----------------------------------------------------------------
   //
   ~Sampler()
   {
      glDeleteSamplers( 1, &id_ );
      id_ = 0;
   }

   // -----------------------------------------------------------------
   //
   void bind( uint32_t slot )
   {
      curSlot_ = slot;
      glBindSampler( curSlot_, id_ );
   }

   // -----------------------------------------------------------------
   //
   void unbind()
   {
      // do we really need this? not sure if this makes sense really.
      glBindSampler( curSlot_, 0 );
   }

private:

   // -----------------------------------------------------------------
   //
   GLuint   id_      = 0;
   uint32_t curSlot_ = 0;
};

}
