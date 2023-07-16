#pragma once

#include <core/graphics/gl/Buffer.h>
#include <core/graphics/gl/ShaderProgram.h>
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
   glm::vec3 tangent_;
};

static constexpr AttribFormat inputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, position_ )},
   {1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, normal_ )},
   {2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, texCoords_ )},
   {3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, tangent_ )},
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
         const std::vector<std::shared_ptr<ow::Texture>>& normalMapTextures,
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
   std::vector<std::shared_ptr<ow::Texture>> normalMapTextures_;

   std::shared_ptr<Buffer> vbo_;
   std::shared_ptr<Buffer> ebo_;
};
}
