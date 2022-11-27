#pragma once

#include "Sampler.h"

namespace ow
{

// =======================================================================
//
Sampler::Sampler( SamplerType type )
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

// =======================================================================
//
Sampler::~Sampler()
{
   glDeleteSamplers( 1, &id_ );
   id_ = 0;
}

// =======================================================================
//
void Sampler::bind( uint32_t slot )
{
   curSlot_ = slot;
   glBindSampler( curSlot_, id_ );
}

// =======================================================================
//
void Sampler::unbind()
{
   // do we really need this? not sure if this makes sense really.
   glBindSampler( curSlot_, 0 );
}
}
