#include <Windows.h> // Glew may need some stuff from here.

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include "core/Engine.h"

// Parameters
static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;

int main( int argc, char** argv )
{

    ow::core::Engine engine( kWIN_WIDTH, kWIN_HEIGHT );
    engine.run();

    return 0;
}