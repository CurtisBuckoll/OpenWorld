#include "Buffer.h"

namespace ow
{
namespace core
{
// =======================================================================
//
Buffer::Buffer( BufferUsage usage,
                uint32_t sizeBytes /*= 0*/,
                uint32_t stride /*= 0*/,
                void* data /*= nullptr*/ )
   : usage_( usage )
   , byteStride_( stride )
{
   switch( usage_ )
   {
   case BufferUsage::VertexBufferObject:
      glGenBuffers( 1, &id_ );
      if( data )
      {
         glBindBuffer( GL_ARRAY_BUFFER, id_ );
         glBufferData( GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW );
         glBindBuffer( GL_ARRAY_BUFFER, 0 );
      }
      break;

   case BufferUsage::ElementBufferObject:
      glGenBuffers( 1, &id_ );
      if( data )
      {
         glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );
         glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW );
         glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
      }
      break;

      // ...
   default:
      break;
   }
}

// =======================================================================
//
Buffer::~Buffer()
{
   switch( usage_ )
   {
   case BufferUsage::VertexBufferObject:
      glDeleteBuffers( 1, &id_ );
      break;

   case BufferUsage::ElementBufferObject:
      glDeleteBuffers( 1, &id_ );
      break;

      // ...
   default:
      break;
   }
}

// =======================================================================
//
void Buffer::bind( uint32_t slot /*= 0*/,
                   uint32_t offset /*= 0*/ )
{
   switch( usage_ )
   {
   case BufferUsage::VertexBufferObject:
      // for now, we stick with binding slot 0. will have to do more
      // to add more slots.
      glBindVertexBuffer( 0, id_, offset, byteStride_ );
      glVertexBindingDivisor( 0, 0 ); // TODO: do we need this??
      break;

   case BufferUsage::ElementBufferObject:
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );
      break;
      // ...
   default:
      break;
   }
}

// =======================================================================
//
void Buffer::unbind()
{
   switch( usage_ )
   {
   case BufferUsage::VertexBufferObject:
      glBindBuffer( GL_ARRAY_BUFFER, 0 ); // TODO: is this correct?
      break;
   case BufferUsage::ElementBufferObject:
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
      break;
      // ...
   default:
      break;
   }
}

}
}
