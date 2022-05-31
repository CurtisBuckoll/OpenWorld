#pragma once

#include <core/graphics/gl/Buffer.h>
#include <core/graphics/gl/Texture.h>

#include <vector>

#include <GLM/glm.hpp>

namespace ow
{

// =======================================================================
//
struct Vertex 
{
   glm::vec3 position_;
   glm::vec3 normal_;
   glm::vec2 texCoords_;
};

struct TypedTexture
{
   std::shared_ptr<core::Texture> texture_;
   std::string type_; // TODO: probably don't use string later, but understand better how we will use this
};

// =======================================================================
//
class Mesh {
public:
   // -----------------------------------------------------------------
   //
   Mesh() = delete;
   Mesh( const Mesh& ) = delete;
   Mesh& operator=( const Mesh& ) = delete;
   Mesh( Mesh&& ) = default;
   Mesh& operator=( Mesh&& ) = default;

   // -----------------------------------------------------------------
   //
   Mesh( const std::vector<Vertex>& vertices,
         const std::vector<uint32_t>& indices,
         const std::vector<TypedTexture>& textures )
      : vertices_( vertices )
      , indices_( indices )
      , textures_( textures ) 
   {
      init( vertices, indices );
   }

   void draw()
   {
      // TODO: textures
      // TODO: also test by passing some dummy vertex/index data from main to sanity check current set up

      // bind the vbo & ebo, for now fixed at slot 0 wtih no offset
      vbo_->bind();
      ebo_->bind();
      glDrawElements( GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
   }

private:
   // -----------------------------------------------------------------
   //
   void init( const std::vector<Vertex>& vertices,
              const std::vector<uint32_t>& indices )
   {
      vbo_ = std::make_shared<ow::core::Buffer>( core::BufferUsage::VertexBufferObject, 
                                                 sizeof( Vertex ) * vertices.size(), 
                                                 sizeof( Vertex ), 
                                                 (void*)vertices.data() );
      ebo_ = std::make_shared<ow::core::Buffer>( ow::core::BufferUsage::ElementBufferObject, 
                                                 sizeof( uint32_t ) * indices.size(),
                                                 sizeof( uint32_t ), 
                                                 (void*)indices.data() );
   }

   // TODO: we don't really need to store these. maybe do so for now and
   // come back to this
   std::vector<Vertex> vertices_;
   std::vector<uint32_t> indices_;
   std::vector<TypedTexture> textures_;

   std::shared_ptr<core::Buffer> vbo_;
   std::shared_ptr<core::Buffer> ebo_;
};
}
