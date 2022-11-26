#pragma once

#include <GL/glew.h>

#include <stdint.h>


namespace ow
{

enum class BufferUsage
{
   VertexBuffer,
   ElementBuffer,
   UniformBuffer,
   BufferUsage_COUNT
};

// =======================================================================
//
class Buffer
{
public:

   // -----------------------------------------------------------------
   //
   Buffer() = delete;
   Buffer( const Buffer& ) = delete;
   Buffer& operator=( const Buffer& ) = delete;
   Buffer( Buffer&& ) = delete;
   Buffer& operator=( Buffer&& ) = delete;

   // -----------------------------------------------------------------
   //
   Buffer( BufferUsage usage, uint32_t sizeBytes = 0, uint32_t stride = 0, const void* data = nullptr );

   // -----------------------------------------------------------------
   //
   ~Buffer();

   // -----------------------------------------------------------------
   //
   void update( void* data, uint32_t sizeBytes, uint32_t offset = 0 );

   // TODO
   // void clear(...);

   // -----------------------------------------------------------------
   //
   void bind( uint32_t slot = 0, uint32_t offset = 0 );

   // -----------------------------------------------------------------
   //
   void unbind();

private:

   // -----------------------------------------------------------------
   //
   BufferUsage usage_;
   GLuint      id_         = 0;
   uint32_t    byteStride_ = 0;
};

}
