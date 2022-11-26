#include "Skybox.h"

namespace
{
// we assume all files that constitute cubemap faces follow this convention so
// that all we need is the root folder containing the cubemap files
// TODO: support other file extensions
static constexpr uint32_t kNumCubeFaces = 6;
static constexpr std::string_view kCubemapFaces[kNumCubeFaces]
{
   "right.jpg",
   "left.jpg",
   "top.jpg",
   "bottom.jpg",
   "front.jpg",
   "back.jpg"
};

static constexpr float kCubeVertices[] = {
   // positions only
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,

   -1.0f, -1.0f,  1.0f,
   -1.0f, -1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

   -1.0f, -1.0f,  1.0f,
   -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,

   -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,

   -1.0f, -1.0f, -1.0f,
   -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
   -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};
}

namespace ow
{
// =======================================================================
//
Skybox::Skybox( const char* folderPath )
{
   const std::string dir = ow::workingDir() + folderPath;
   std::vector<std::string> filePaths( kNumCubeFaces );
   for( uint32_t i = 0; i < kNumCubeFaces; ++i )
   {
      filePaths[i] = dir + std::string( kCubemapFaces[i] );
   }
   cubemapTexture_ = std::make_shared<ow::Texture>( filePaths );
   cubeVertexData_ = std::make_shared<ow::Buffer>( BufferUsage::VertexBuffer, sizeof( kCubeVertices ), 3 * sizeof( float ), kCubeVertices );
}

// =======================================================================
//
void Skybox::draw()
{
   cubeVertexData_->bind();
   cubemapTexture_->bind( 0, nullptr );
   glDepthMask( GL_FALSE );
   glDepthFunc( GL_LEQUAL );
   glDrawArrays( GL_TRIANGLES, 0, sizeof( kCubeVertices ) / ( 3 * sizeof( float ) ) );
   glDepthMask( GL_TRUE );
}
}
