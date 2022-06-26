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
         const std::vector<std::shared_ptr<ow::core::Texture>>& diffuseTextures,
         const std::vector<std::shared_ptr<ow::core::Texture>>& specularTextures )
      : vertices_( vertices )
      , indices_( indices )
      , diffuseTextures_( diffuseTextures )
      , specularTextures_( specularTextures )
   {
      sampler_ = std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips );
      init( vertices, indices );
   }

   void draw()
   {

      // bind the vbo & ebo, for now fixed at slot 0 wtih no offset
      vbo_->bind();
      ebo_->bind();
      
      // just test with diffuse texture for now
      //diffuseTextures_[0]->bind( 0, sampler_ );

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

   // TODO: change how we use samplers so we don't make one per mesh but instead share.
   std::shared_ptr<ow::core::Sampler> sampler_;

   // just bind the first of each for now
   std::vector<std::shared_ptr<ow::core::Texture>> diffuseTextures_;
   std::vector<std::shared_ptr<ow::core::Texture>> specularTextures_;

   std::shared_ptr<core::Buffer> vbo_;
   std::shared_ptr<core::Buffer> ebo_;
};
}
