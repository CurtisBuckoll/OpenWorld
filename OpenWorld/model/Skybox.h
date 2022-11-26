#pragma once

#include "core/graphics/gl/Buffer.h"
#include "core/graphics/gl/Texture.h"
#include "core/Platform.h"

#include "IRenderable.h"

#include <string_view>

namespace ow
{

// =======================================================================
//
class Skybox : public IRenderable
{
public:
   // -----------------------------------------------------------------
   //
   Skybox() = delete;
   Skybox( const Skybox& ) = delete;
   Skybox& operator=( const Skybox& ) = delete;
   Skybox( Skybox&& ) = default;
   Skybox& operator=( Skybox&& ) = default;

   // -----------------------------------------------------------------
   //
   Skybox( const char* folderPath );

   // -----------------------------------------------------------------
   //
   virtual void draw() override;

private:

   // -----------------------------------------------------------------
   //
   std::shared_ptr<ow::Texture> cubemapTexture_;
   std::shared_ptr<ow::Buffer> cubeVertexData_;
};
}
