#pragma once

#include <assert.h>
#include <string>

// TODO: move this elsewhere?
#define OW_ASSERT(ARG) assert(ARG)

// -----------------------------------------------------------------
//
namespace ow
{
namespace core
{

// -----------------------------------------------------------------
//
std::string workingDir();

}
}
