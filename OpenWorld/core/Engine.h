#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

// -----------------------------------------------------------------
//
namespace ow
{
namespace core
{

// =======================================================================
//
enum class state { RUN, EXIT };

// =======================================================================
//
class Engine
{

public:

    // -----------------------------------------------------------------
    //
    Engine( int win_width,
            int win_height );

    // -----------------------------------------------------------------
    //
    void run();

    // -----------------------------------------------------------------
    //
    Engine()                          = delete;
    ~Engine()                         = default;
    Engine( const Engine& )           = delete;
    Engine( Engine&& )                = delete;
    Engine operator=( const Engine& ) = delete;
    Engine operator=( Engine&& )      = delete;

private:

    // -----------------------------------------------------------------
    //
    void init();

    // -----------------------------------------------------------------
    //
    void loop();

    // -----------------------------------------------------------------
    //
    void process_system_exit();

    // -----------------------------------------------------------------
    //
    SDL_Window* window_;
    int         width_;
    int         height_;
    state       state_;
};

}
}
