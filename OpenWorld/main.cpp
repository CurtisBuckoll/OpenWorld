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
   bool init = false;
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); 
   (void)io;
   ImGui::StyleColorsDark();
   init = ImGui_ImplSDL2_InitForOpenGL( engine.window(), engine.getGLContext() );
   init = ImGui_ImplOpenGL3_Init( "#version 440" );

   // imgui variables
   bool enableNormalMapping = true;
   float size = 1.0f;
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

   RenderConstants renderConstants;
   renderConstants.proj_ = glm::perspective( glm::radians( 60.0f ), (float)kWinWidth / (float)kWinHeight, 0.1f, 5000.0f );
   renderConstants.view_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -3.0f ) );
   renderConstants.model_ = glm::mat4( 1.0f );
   renderConstants.bEnableNormalMaps_ = 1;

   ow::Buffer constantBuffer( ow::BufferUsage::UniformBuffer, sizeof( RenderConstants ), 0, nullptr );

   // input state and camera test
   InputState inputState;
   Camera3D cam;
   cam.init( glm::vec3( 0.0f, 0.0f, 3.0f ) );

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

      renderConstants.view_ = cam.viewMatrix();
      renderConstants.viewPos_ = cam.viewPosition();
      renderConstants.bEnableNormalMaps_ = enableNormalMapping;

      glClearDepth( 1.0 );
      glClearColor( 0.2f, 0.3f, 0.5f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glEnable( GL_DEPTH_TEST );

      // new frame for imgui
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();

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

      shaderProgram.use();
      glDepthFunc( GL_LESS );
      constantBuffer.update( &renderConstants, sizeof( RenderConstants ) );
      constantBuffer.bind( 0 );
      backpackModel.draw();
      shaderProgram.unuse();

      // 3. render skybox last for perf

      skyboxShaderProgram.use();
      constantBuffer.bind( 0 );
      skybox->draw();
      skyboxShaderProgram.unuse();

      // imgui window population
      ImGui::Begin( "My name is window, ImGUI window" );
      // Text that appears in the window
      ImGui::Text( "OpenWorld" );
      // Checkbox that appears in the window
      ImGui::Checkbox( "Enable Normal Mapping", &enableNormalMapping );
      // Slider that appears in the window
      ImGui::SliderFloat( "Slider", &size, 0.5f, 2.0f );
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