#include "Core/Utils/IOUtils.h"

#include <fstream>

[[nodiscard]]
std::vector<char> ReadFile(const fs::path& filepath)
{
  std::vector<char> result;

  try
  {
    // Open the file with the cursor at the end
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
      throw std::runtime_error("File can't be opened.");
    }

    // The location of the cursor tells the size of
    // the file in bytes
    unsigned long long filesize = static_cast<unsigned long long>(file.tellg());

    if (filesize < 0)
    {
      throw std::runtime_error("File length is negative.");
    }

    result.resize(filesize);

    // Place the fie cursor at the beginning
    file.seekg(0);

    // Load the entire file into the buffer
    file.read(result.data(), filesize);

    // Close the file
    file.close();
  }
  catch (const std::exception& e)
  {
    printf("%s", e.what());
    return result;
  }

  // Return true
  return result;
}
