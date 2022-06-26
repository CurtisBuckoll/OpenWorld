#include "Buffer.h"

#include "io/Logging.h"

namespace ow
{

static GLenum getGlBufferUsage( BufferUsage usage )
{
   switch( usage )
   {
   case BufferUsage::VertexBuffer:  return GL_ARRAY_BUFFER;
   case BufferUsage::ElementBuffer: return GL_ELEMENT_ARRAY_BUFFER;
   case BufferUsage::UniformBuffer: return GL_UNIFORM_BUFFER;
   default:
      OW_LOG( ERRO, "unrecognized buffer type %d", (int32_t)usage );
      return 0;
   }
}

// =======================================================================
//
Buffer::Buffer( BufferUsage usage,
                uint32_t sizeBytes /*= 0*/,
                uint32_t stride /*= 0*/,
                void* data /*= nullptr*/ )
   : usage_( usage )
   , byteStride_( stride )
{
   glGenBuffers( 1, &id_ );

   GLenum bufferUsage = getGlBufferUsage( usage_ );
   glBindBuffer( bufferUsage, id_ );
   glBufferData( bufferUsage, sizeBytes, data, GL_STATIC_DRAW );
   glBindBuffer( bufferUsage, 0 );

   //switch( usage_ )
   //{
   //case BufferUsage::VertexBuffer:
   //   if( data )
   //   {
   //      glBindBuffer( GL_ARRAY_BUFFER, id_ );
   //      glBufferData( GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW );
   //      glBindBuffer( GL_ARRAY_BUFFER, 0 );
   //   }
   //   break;

   //case BufferUsage::ElementBuffer:
   //   if( data )
   //   {
   //      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );
   //      glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW );
   //      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
   //   }
   //   break;

   //case BufferUsage::UniformBuffer:
   //   glBindBuffer( GL_UNIFORM_BUFFER, id_ );
   //   glBufferData( GL_UNIFORM_BUFFER, sizeBytes, data, GL_STATIC_DRAW );
   //   glBindBuffer( GL_UNIFORM_BUFFER, 0 );
   //   break;

   //   // ...
   //default:
   //   break;
   //}
}

// =======================================================================
//
Buffer::~Buffer()
{
   glDeleteBuffers( 1, &id_ );
   id_ = 0;

   //switch( usage_ )
   //{
   //case BufferUsage::VertexBuffer:
   //   glDeleteBuffers( 1, &id_ );
   //   break;

   //case BufferUsage::ElementBuffer:
   //   glDeleteBuffers( 1, &id_ );
   //   break;

   //   // ...
   //default:
   //   break;
   //}

   //id_ = 0;
}

void Buffer::update(void* data, uint32_t offset, uint32_t sizeBytes)
{
   GLenum bufferUsage = getGlBufferUsage( usage_ );
   glBindBuffer( bufferUsage, id_ );
   glBufferSubData( bufferUsage, offset, sizeBytes, data );
   glBindBuffer( bufferUsage, 0 );
}

// =======================================================================
//
void Buffer::bind( uint32_t slot /*= 0*/,
                   uint32_t offset /*= 0*/ )
{
   switch( usage_ )
   {
   case BufferUsage::VertexBuffer:
      // for now, we stick with binding slot 0. will have to do more
      // to add more slots.
      glBindVertexBuffer( 0, id_, offset, byteStride_ );
      glVertexBindingDivisor( 0, 0 ); // TODO: do we need this??
      break;

   case BufferUsage::ElementBuffer:
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );
      break;
      
   case BufferUsage::UniformBuffer:
      glBindBufferBase( GL_UNIFORM_BUFFER, slot, id_ );

   default:
      // shouldn't ever get here
      break;
   }
}

// =======================================================================
//
void Buffer::unbind()
{
   GLenum bufferUsage = getGlBufferUsage( usage_ );
   glBindBuffer( bufferUsage, 0 );

   //switch( usage_ )
   //{
   //case BufferUsage::VertexBuffer:
   //   glBindBuffer( GL_ARRAY_BUFFER, 0 ); // TODO: is this correct?
   //   break;
   //case BufferUsage::ElementBuffer:
   //   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
   //   break;
   //   // ...
   //default:
   //   break;
   //}
}

}
