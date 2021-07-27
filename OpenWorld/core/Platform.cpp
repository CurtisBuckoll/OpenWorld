#include "Platform.h"

#include <iostream>
#include <stdio.h>  // defines FILENAME_MAX

#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace core
{

// =======================================================================
//
std::string workingDir()
{
#if _WIN32
   // can probably store this once instead of converting, but should
   // be fine if not a bottleneck, which should be unlikely
   TCHAR buffer[MAX_PATH];
   GetCurrentDirectory( MAX_PATH, buffer );
   std::wstring wstr( &buffer[0] );
   return std::string( wstr.begin(), wstr.end() );
#else 
   return std::string( "" );
#endif// _WIN32
}

}