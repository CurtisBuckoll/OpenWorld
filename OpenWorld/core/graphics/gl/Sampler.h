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
   Sampler( Sampler&& ) = delete;
   Sampler& operator=( Sampler&& ) = delete;

   // -----------------------------------------------------------------
   //
   Sampler( SamplerType type );

   // -----------------------------------------------------------------
   //
   ~Sampler();

   // -----------------------------------------------------------------
   //
   void bind( uint32_t slot );

   // -----------------------------------------------------------------
   //
   void unbind();

private:

   // -----------------------------------------------------------------
   //
   GLuint   id_      = 0;
   uint32_t curSlot_ = 0;
};

}
