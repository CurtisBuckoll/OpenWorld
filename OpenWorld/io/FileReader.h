#pragma once

#include <memory>
#include <string>
#include <fstream>

namespace ow
{

class FileReader
{
public:
   FileReader() = default;
   FileReader( const FileReader& ) = delete;
   FileReader& operator=( const FileReader& ) = delete;

   ~FileReader();

   FileReader( const std::string& absPath, bool isBinary );

   uint8_t* read( const std::string& absPath, bool isBinary );

   uint8_t* data();

private:

   void openFile( const std::string& absPath, bool isBinary );
   void closeFile();

   std::ifstream f_;
   std::unique_ptr<uint8_t[]> data_;
};

}
