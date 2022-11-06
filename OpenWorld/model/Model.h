#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <unordered_map>

#include "io/Logging.h"
#include "Mesh.h"

#include "core/Platform.h"

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
   // note: other options for import that we may want to add as parameters 
   // along with the ones that we are hardcoding:
   // 
   // aiProcess_GenNormals
   // aiProcess_OptimizeMeshes
   // aiProcess_SplitLargeMeshes
   Model( const std::string& relFilePath )
      : sampler_(std::make_shared<ow::core::Sampler>( ow::core::SamplerType::LinFilterLinMips ))
   {
      std::filesystem::path path( relFilePath );
      path.remove_filename();
      assetDirectory_ = ow::core::workingDir() + path.string();

      const auto assetFilePath = ow::core::workingDir() + relFilePath;

      OW_LOG( INFO, "loading assimp model: %s", assetFilePath.c_str() );
      OW_LOG( INFO, "asset directory: %s", assetDirectory_.c_str() );

      Assimp::Importer importer;
      auto scene = importer.ReadFile( assetFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
      if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
      {
         OW_LOG( ERRO, "Assimp error loading model: %s", assetFilePath.c_str() );
      }
      else
      {
         meshes_.reserve( scene->mNumMeshes );
         processNode(scene->mRootNode, scene);
      }
   }

   // -----------------------------------------------------------------
   //
   void draw()
   {
      for( auto& mesh : meshes_ )
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
      std::vector<Vertex> vertices;
      std::vector<uint32_t> indices;
      std::vector<std::shared_ptr<ow::core::Texture>> diffTextures;
      std::vector<std::shared_ptr<ow::core::Texture>> specTextures;

      for( uint32_t i = 0; i < mesh->mNumVertices; i++ )
      {
         Vertex vertex;
         glm::vec3 v;
         v.x = mesh->mVertices[i].x;
         v.y = mesh->mVertices[i].y;
         v.z = mesh->mVertices[i].z;
         vertex.position_ = v;

         v.x = mesh->mNormals[i].x;
         v.y = mesh->mNormals[i].y;
         v.z = mesh->mNormals[i].z;
         vertex.normal_ = v;

         // the mesh may not contain texture coords, so check first. also,
         // up to 8 different sets of texture coords are supported, but we
         // will only take from the first
         if( mesh->mTextureCoords[0] )
         {
            glm::vec2 v2;
            v2.x = mesh->mTextureCoords[0][i].x;
            v2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords_ = v2;
         }
         else
         {
            vertex.texCoords_ = glm::vec2{};
         }

         vertices.push_back( vertex );
      }

      for( unsigned int i = 0; i < mesh->mNumFaces; i++ )
      {
         aiFace face = mesh->mFaces[i];
         for( unsigned int j = 0; j < face.mNumIndices; j++ )
         {
            indices.push_back( face.mIndices[j] );
         }
      }

      if( mesh->mMaterialIndex >= 0 )
      {
         aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
         diffTextures = loadMaterialTextures( material, aiTextureType_DIFFUSE );
         specTextures = loadMaterialTextures( material, aiTextureType_SPECULAR );
      }

      return ow::Mesh( vertices, indices, diffTextures, specTextures, sampler_ );
   }

   // -----------------------------------------------------------------
   //
   std::vector<std::shared_ptr<ow::core::Texture>> loadMaterialTextures( aiMaterial* mat,
                                                                         aiTextureType type )
   {
      std::vector<std::shared_ptr<ow::core::Texture>> textures;
      for( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
      {
         // this literally gives us only file name, so we have to prepend
         // with the full path
         aiString filename;
         mat->GetTexture( type, i, &filename );
         
         std::string path = assetDirectory_ + filename.C_Str();
         std::shared_ptr<ow::core::Texture> tex;

         // check if in cache and reuse if so
         auto found = textureCache_.find( path );
         if( found == textureCache_.end() )
         {
            tex = std::make_shared<ow::core::Texture>( path, true );
            textureCache_.insert( { path, tex } );
         }
         else
         {
            tex = found->second;
         }
         textures.push_back( tex );
      }
      return textures;
   }

   // -----------------------------------------------------------------
   //
   std::vector<ow::Mesh> meshes_;
   std::string assetDirectory_;
   std::shared_ptr<ow::core::Sampler> sampler_;

   std::unordered_map<std::string, std::shared_ptr<ow::core::Texture>> textureCache_;
};
}
