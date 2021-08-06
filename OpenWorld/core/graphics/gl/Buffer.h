#pragma once

#include <GL/glew.h>

#include <stdint.h>


namespace ow
{
namespace core
{

enum class BufferUsage
{
   VertexBufferObject,
   ElementBufferObject,
   COUNT
};

class Buffer
{
public:

   // -----------------------------------------------------------------
   //
   Buffer() = delete;
   Buffer( const Buffer& ) = delete;
   Buffer& operator=( const Buffer& ) = delete;
   Buffer( const Buffer&& ) = delete;
   Buffer& operator=( const Buffer&& ) = delete;

   // -----------------------------------------------------------------
   //
   Buffer( BufferUsage usage, uint32_t sizeBytes = 0, uint32_t stride = 0, void* data = nullptr );

   // -----------------------------------------------------------------
   //
   ~Buffer();

   // TODO:
   // void update(...);
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
   GLuint      id_ = 0;
   BufferUsage usage_;
   uint32_t    byteStride_;
};

}
}
