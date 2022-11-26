#pragma once

namespace ow
{

// =======================================================================
// IRenderable provides an interface to draw() for any classes that
// implement it
class IRenderable
{
public:
   // -----------------------------------------------------------------
   //
   virtual ~IRenderable() = default;

   // -----------------------------------------------------------------
   //
   virtual void draw() = 0;
};
}