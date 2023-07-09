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
         const std::vector<std::shared_ptr<ow::Texture>>& diffuseTextures,
         const std::vector<std::shared_ptr<ow::Texture>>& specularTextures,
         std::shared_ptr<ow::Sampler> sampler );

   void draw();

private:
   // -----------------------------------------------------------------
   //
   void init( const std::vector<Vertex>& vertices,
              const std::vector<uint32_t>& indices );

   // TODO: we don't really need to store these. maybe do so for now and
   // come back to this
   std::vector<Vertex> vertices_;
   std::vector<uint32_t> indices_;

   // TODO: change how we use samplers so we don't make one per mesh but instead share.
   std::shared_ptr<ow::Sampler> sampler_;

   // just bind the first of each for now
   std::vector<std::shared_ptr<ow::Texture>> diffuseTextures_;
   std::vector<std::shared_ptr<ow::Texture>> specularTextures_;

   std::shared_ptr<Buffer> vbo_;
   std::shared_ptr<Buffer> ebo_;
};
}
