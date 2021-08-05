#include "FileReader.h"

#include <iostream>
#include <unordered_map>

namespace ow
{

FileReader::~FileReader()
{
   closeFile();
}

FileReader::FileReader( const std::string& absPath, bool isBinary )
{
   openFile( absPath, isBinary );
}

uint8_t* FileReader::read( const std::string& absPath, bool isBinary )
{
   closeFile();
   openFile( absPath, isBinary );
   return data();
}

uint8_t* FileReader::data()
{
   return data_.get();
}

void FileReader::closeFile()
{
   if( f_.is_open() )
   {
      f_.close();
   }
   data_.reset();
}

void FileReader::openFile( const std::string& absPath, bool isBinary )
{
   f_.open( absPath.c_str(), isBinary ? std::ios_base::binary : std::ios_base::in );

   if( !f_.is_open() )
   {
      // TODO: error properly using errors class/logs
      std::cout << "(LOG) ERROR: Failed to load shader file: " << absPath << std::endl;
      return;
   }

   f_.seekg( 0, std::ios::end );
   uint64_t numBytes = f_.tellg();
   f_.seekg( 0, std::ios_base::beg );
   data_ = std::make_unique<uint8_t[]>( numBytes );
   f_.read( reinterpret_cast<char*>(data_.get()), numBytes );
}

}
