// including windows.h messes up some stuff with min/max macros, so
// define NOMINMAX to subvert that. glew may need some things from
// windows.h
// we can probably move these includes to better places though later, so
// make sure this is adressed better if it can be
#define NOMINMAX
#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"
#include "core/Platform.h"

// temp
#include "core/graphics/gl/Texture.h"
#include "core/graphics/gl/Buffer.h"
#include "core/graphics/gl/Framebuffer.h"
#include "model/Model.h"
#include "model/Skybox.h"

#include "core/Camera3D.h"
#include "core/InputState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "io/Logging.h"

struct RenderConstants
{
   glm::mat4 model_;
   glm::mat4 view_;
   glm::mat4 proj_;
   glm::vec3 viewPos_;
   uint32_t bEnableNormalMaps_;
};

struct LightConstants
{
   glm::mat4 lightView_;
   glm::mat4 lightProj_;
};

struct Vertex
{
   float pos[3];
   float normal[3];
   float uv[2];
   float tangent[3];
};

static constexpr ow::AttribFormat skyboxInputLayout[] = {
   {0, 3, GL_FLOAT, GL_FALSE, 0},
};

static constexpr ow::AttribFormat fullscreenInputLayout[] = {
   {0, 2, GL_FLOAT, GL_FALSE, 0},
   {1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float)},
};

// TODO: read from file
// parameters
static const int32_t kWinWidth = 1200;
static const int32_t kWinHeight = 700;

// =======================================================================
//
int main( int argc, char** argv )
{
   OW_LOG( INFO, "Lauching Open World" );
   OW_LOG( INFO, "initializing window with window resolution: %dx%d", kWinWidth, kWinHeight );

   ow::Engine engine( kWinWidth, kWinHeight );
   engine.init();

   // Initialize ImGUI
   bool initImgui = false;
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); 
   (void)io;
   ImGui::StyleColorsDark();
   initImgui = ImGui_ImplSDL2_InitForOpenGL( engine.window(), engine.getGLContext() );
   initImgui = ImGui_ImplOpenGL3_Init( "#version 440" );

   // imgui variables
   bool enableNormalMapping = true;
   glm::vec3 lightPos( 30.0f, 30.0f, 30.0f );
   float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };

   ow::ShaderProgram shaderProgram( "simpleShader_vs",
                                    "simpleShader_fs",
                                    ow::inputLayout,
                                    sizeof( ow::inputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram reflectShaderProgram( "simpleShader_vs",
                                           "reflective_fs",
                                           ow::inputLayout,
                                           sizeof( ow::inputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram skyboxShaderProgram( "skyboxShader_vs",
                                          "skyboxShader_fs",
                                          skyboxInputLayout,
                                          sizeof( skyboxInputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram renderShadowsProgram( "renderDepth_vs",
                                           "renderDepth_fs",
                                           ow::inputLayout,
                                           sizeof( ow::inputLayout ) / sizeof( ow::AttribFormat ) );

   ow::ShaderProgram fullscreenBlitProgram( "fullscreenProgram_vs",
                                            "fullscreenProgram_fs",
                                            fullscreenInputLayout,
                                            sizeof( fullscreenInputLayout ) / sizeof( ow::AttribFormat ) );

   auto shadowFb = std::make_shared<ow::Framebuffer>( 2048, 2048, true );

   auto sampler = std::make_shared<ow::Sampler>( ow::SamplerType::LinFilterLinMips );

   // test making a framebuffer
   auto fb = std::make_shared<ow::Framebuffer>( kWinWidth, kWinHeight );

   //ow::Model backpackModel = ow::Model( "assets\\test\\backpack\\backpack.obj" );
   //ow::Model backpackModel = ow::Model( "assets\\sponza\\scene.gltf" );
   //ow::Model backpackModel = ow::Model( "assets\\curtains\\NewSponza_Curtains_glTF.gltf" );
   ow::Model backpackModel = ow::Model( "assets\\sponza-gltf-pbr\\sponza.glb" );

   // "C:\OpenWorld\OpenWorld\assets\sponza\New_Sponza_001.gltf"

   auto skybox = std::make_shared<ow::Skybox>( "assets\\test\\brudslojan_skybox\\" );

   OW_LOG_INFO( "starting main loop" );

   glm::mat4 renderProjMat = glm::perspective( glm::radians( 60.0f ), (float)kWinWidth / (float)kWinHeight, 0.1f, 100.0f );

   RenderConstants renderConstants;
   renderConstants.proj_ = renderProjMat;
   renderConstants.view_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, -40.0f, -3.0f ) );
   renderConstants.model_ = glm::mat4( 1.0f );
   renderConstants.bEnableNormalMaps_ = 1;

   ow::Buffer constantBuffer( ow::BufferUsage::UniformBuffer, sizeof( RenderConstants ), 0, nullptr );
   ow::Buffer lightConstantBuffers( ow::BufferUsage::UniformBuffer, sizeof( LightConstants ), 0, nullptr );

   // input state and camera test
   InputState inputState;
   Camera3D cam;
   cam.init( glm::vec3( 0.0f, 3.0f, 3.0f ) );

   bool running = true;
   while( running )
   {
      //fb->bind();

      running = inputState.pollForEvents();
      if( !io.WantCaptureMouse )
      {
         // only process input when not hovering over ui
         cam.processInput( inputState );
      }

      // new frame for imgui
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();

      // 0. Render shadows
      // hack this in for now, we will definitely need to fix this up to support multiple lights + plumbing the data in properly
      glViewport( 0, 0, 2048, 2048 ); // TODO do this some better way

      renderShadowsProgram.use();
      shadowFb->bind();
      glClearDepth( 1.0 );
      glClear( GL_DEPTH_BUFFER_BIT );
      glEnable( GL_DEPTH_TEST );
      glDepthFunc( GL_LESS );



      glm::mat4 lightView = glm::lookAt( lightPos,
                                         lightPos + glm::vec3( -1.0f, -1.0f, -1.0f ),
                                         glm::vec3( 0.0f, 1.0f, 0.0f ) );
      glm::mat4 lightProjection = glm::ortho( -50.0f, 50.0f, -50.0f, 50.0f, 1.0f, 100.0f );

      renderConstants.view_ = lightView;
      renderConstants.proj_ = lightProjection;
      renderConstants.bEnableNormalMaps_ = enableNormalMapping;

      constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      constantBuffer.bind( 0 );
      backpackModel.draw();

      shadowFb->unbind();


      // test for visualize, can delete this
#if 0
      float quadVertices_delete [] = {
         // positions   // texCoords
         -1.0f,  1.0f,  0.0f, 1.0f,
         -1.0f, -1.0f,  0.0f, 0.0f,
          1.0f, -1.0f,  1.0f, 0.0f,

         -1.0f,  1.0f,  0.0f, 1.0f,
          1.0f, -1.0f,  1.0f, 0.0f,
          1.0f,  1.0f,  1.0f, 1.0f
      };

      auto quad_buffer = std::make_shared<ow::Buffer>( ow::BufferUsage::VertexBuffer, sizeof( quadVertices_delete ), 4 * sizeof(float), quadVertices_delete);

      fullscreenBlitProgram.use();
      glViewport( 0, 0, kWinWidth, kWinHeight ); // TODO do this some better way
      //glBindVertexArray( quad_buffer );
      quad_buffer->bind();
      glDisable( GL_DEPTH_TEST );
      //glBindTexture( GL_TEXTURE_2D, shadowFb->depthTexId() );
      shadowFb->depthTex_->bind( 0, sampler );
      glDrawArrays( GL_TRIANGLES, 0, 6 );
#endif

      // 1. reflective rendering for fun:

      //reflectShaderProgram.use();
      //glEnable( GL_DEPTH_TEST );
      //glDepthFunc( GL_LESS );
      //constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      //constantBuffer.bind( 0 );
      //cubemapTexture->bind( 2, sampler );

      //backpackModel.draw();
      //reflectShaderProgram.unuse();


      // 2. usual rendering of backpack:
#if 1
      glViewport( 0, 0, kWinWidth, kWinHeight ); // TODO do this some better way

      renderConstants.view_ = cam.viewMatrix();
      renderConstants.viewPos_ = cam.viewPosition();
      renderConstants.proj_ = renderProjMat;
      renderConstants.bEnableNormalMaps_ = enableNormalMapping;

      LightConstants lightConstants{};
      lightConstants.lightView_ = lightView;
      lightConstants.lightProj_ = lightProjection;

      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.5f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glEnable( GL_DEPTH_TEST );

      shaderProgram.use();
      glDepthFunc( GL_LESS );
      constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      lightConstantBuffers.update( &lightConstants, sizeof( LightConstants ) );

      constantBuffer.bind( 0 );
      lightConstantBuffers.bind( 1 );

      shadowFb->depthTex_->bind( 3, sampler );

      backpackModel.draw();
      shaderProgram.unuse();
#endif

      // 3. render skybox last for perf
      glEnable( GL_DEPTH_TEST );
      skyboxShaderProgram.use();
      constantBuffer.bind( 0 );
      skybox->draw();
      skyboxShaderProgram.unuse();


      // imgui window population
      ImGui::Begin( "OpenWorld" );
      // Text that appears in the window
      ImGui::Text( "OpenWorld" );
      // Checkbox that appears in the window
      ImGui::Checkbox( "Enable Normal Mapping", &enableNormalMapping );
      // Slider that appears in the window
      ImGui::SliderFloat( "Light pos x", &lightPos.x, -50.0f, 50.0f );
      ImGui::SliderFloat( "Light pos y", &lightPos.y, -50.0f, 50.0f );
      ImGui::SliderFloat( "Light pos z", &lightPos.z, -50.0f, 50.0f );
      // Fancy color editor that appears in the window
      ImGui::ColorEdit4( "Colour", color );
      // Ends the window
      ImGui::End();

      if( enableNormalMapping == false )
      {
         OW_LOG_INFO( "checkbox false" );
      }

      // Renders the ImGUI elements
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

      SDL_GL_SwapWindow( engine.window() );
   }

   //engine.loop();

   // Deletes all ImGUI instances
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplSDL2_Shutdown();
   ImGui::DestroyContext();

   return 0;
}