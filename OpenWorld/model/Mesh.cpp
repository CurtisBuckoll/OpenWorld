#pragma once

#include "Mesh.h"

#include <core/graphics/gl/Buffer.h>
#include <core/graphics/gl/Texture.h>
#include <io/Logging.h>

#include <vector>

#include <GLM/glm.hpp>

namespace ow
{

// =======================================================================
//
Mesh::Mesh( const std::vector<Vertex>& vertices,
      const std::vector<uint32_t>& indices,
      const std::vector<std::shared_ptr<ow::Texture>>& diffuseTextures,
      const std::vector<std::shared_ptr<ow::Texture>>& specularTextures,
      const std::vector<std::shared_ptr<ow::Texture>>& normalMapTextures,
      std::shared_ptr<ow::Sampler> sampler )
   : vertices_( vertices )
   , indices_( indices )
   , diffuseTextures_( diffuseTextures )
   , specularTextures_( specularTextures )
   , normalMapTextures_( normalMapTextures )
   , sampler_( sampler )
{
   //sampler_ = std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips );
   init( vertices, indices );
}

// =======================================================================
//
void Mesh::draw()
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
      specularTextures_[0]->bind( 1, sampler_ );
   }
   else
   {
      OW_LOG( WARN, "no specular textures available for mesh" );
   }
   if( normalMapTextures_.size() )
   {
      normalMapTextures_[0]->bind( 2, sampler_ );
   }
   else
   {
      OW_LOG( WARN, "no normal maps available for mesh" );
   }

   // bind the vbo & ebo, for now fixed at slot 0 with no offset
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
}

// =======================================================================
//
void Mesh::init( const std::vector<Vertex>& vertices,
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
}