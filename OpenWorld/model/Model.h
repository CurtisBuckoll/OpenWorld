#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "io/Logging.h"
#include "Mesh.h"

namespace ow
{

// =======================================================================
//
class Model
{
public:
   // -----------------------------------------------------------------
   //
   Model() = delete;
   Model( const Model& ) = delete;
   Model& operator=( const Model& ) = delete;
   Model( Model&& ) = default;
   Model& operator=( Model&& ) = default;

   // -----------------------------------------------------------------
   //
   Model( const std::string& filePath )
   {
      // note: other options for import that we may want to add as parameters 
      // along with the ones that we are hardcoding:
      // 
      // aiProcess_GenNormals
      // aiProcess_OptimizeMeshes
      // aiProcess_SplitLargeMeshes

      Assimp::Importer importer;
      auto scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
      if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
      {
         OW_LOG( ERRO, "Assimp error loading model: %s", filePath.c_str() );
      }
      else
      {
         meshes_.reserve( scene->mNumMeshes );
         processNode(scene->mRootNode, scene);
      }
   }

   // -----------------------------------------------------------------
   //
   void draw() const
   {
      for( const auto& mesh : meshes_ )
      {
         mesh.draw();
      }
   }

private:
   // -----------------------------------------------------------------
   //
   void processNode( aiNode* node, const aiScene* scene )
   {
      for( uint32_t i = 0; i < node->mNumMeshes; ++i )
      {
         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
         meshes_.push_back( processMesh( mesh, scene ) );
      }
      // then do the same for each of its children
      for( uint32_t i = 0; i < node->mNumChildren; i++ )
      {
         processNode( node->mChildren[i], scene );
      }
   }

   // -----------------------------------------------------------------
   //
   Mesh processMesh( aiMesh* mesh, const aiScene* scene )
   {
      //return Mesh()
   }

   // -----------------------------------------------------------------
   //
   std::vector<ow::TypedTexture> loadMaterialTextures( aiMaterial* mat,
                                                       aiTextureType type,
                                                       std::string typeName )
   {
      return std::vector<ow::TypedTexture>{};
   }

   // -----------------------------------------------------------------
   //
   std::vector<ow::Mesh> meshes_;
};
}
