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
         const std::vector<std::shared_ptr<ow::core::Texture>>& specularTextures,
         std::shared_ptr<ow::core::Sampler> sampler)
      : vertices_( vertices )
      , indices_( indices )
      , diffuseTextures_( diffuseTextures )
      , specularTextures_( specularTextures )
      , sampler_(sampler)
   {
      //sampler_ = std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips );
      init( vertices, indices );
   }

   void draw()
   {
      // for now, just bind first available diffuse & specular textures
      if( diffuseTextures_.size() )
      {
         diffuseTextures_[0]->bind( 0, sampler_ );
      }
      else
      {
         OW_LOG( WARN, "no diffuse textures available for mesh" );
      }
      if( specularTextures_.size() )
      {
         diffuseTextures_[0]->bind( 1, sampler_ );
      }
      else
      {
         OW_LOG( WARN, "no specular textures available for mesh" );
      }

      //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

      // bind the vbo & ebo, for now fixed at slot 0 wtih no offset
      vbo_->bind();
      if( indices_.size() > 0 )
      {
         ebo_->bind();
         glDrawElements( GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0 );
      }
      else
      {
         glDrawArrays( GL_TRIANGLES, 0, vertices_.size() );
      }

      // just test with diffuse texture for now
      //diffuseTextures_[0]->bind( 0, sampler_ );
   }

private:
   // -----------------------------------------------------------------
   //
   void init( const std::vector<Vertex>& vertices,
              const std::vector<uint32_t>& indices )
   {
      vbo_ = std::make_shared<ow::Buffer>( BufferUsage::VertexBuffer,
                                           sizeof( Vertex ) * vertices.size(),
                                           sizeof( Vertex ),
                                           (void*)vertices.data() );
      if( indices.size() > 0 )
      {
         ebo_ = std::make_shared<ow::Buffer>( BufferUsage::ElementBuffer,
                                              sizeof( uint32_t ) * indices.size(),
                                              sizeof( uint32_t ),
                                              (void*)indices.data() );
      }
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

   std::shared_ptr<Buffer> vbo_;
   std::shared_ptr<Buffer> ebo_;
};
}
