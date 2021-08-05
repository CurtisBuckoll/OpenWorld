#pragma once

#include <memory>
#include <string>
#include <fstream>

namespace ow
{
enum class ReadMode
{
   Binary,
   Text
};

class FileReader
{
public:
   FileReader() = delete;

   FileReader( const std::string& relPath, ReadMode mode );
   ~FileReader();

   uint8_t* data();

   void close();

private:

   std::ifstream f_;
   std::unique_ptr<uint8_t> data_;
};

}
