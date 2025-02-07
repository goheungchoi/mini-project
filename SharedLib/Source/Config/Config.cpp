#include "Shared/Config/Config.h"

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

static const char* GetProjectDirectory()
{
  static const std::string projectDir = []() -> std::string {
    fs::path cwd = fs::current_path();
    fs::path projectDir;
    if (fs::exists(cwd / "Asset") && fs::exists(cwd / "Library"))
    {
      projectDir = cwd;
    }
    else
    { // Go to the parent path of the current path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Asset") && fs::exists(cwd / "Library"))
      {
        projectDir = cwd;
      }
      else
      {
        abort();
      }
    }
    return projectDir.string();
  }();

  return projectDir.c_str();
}

static const char* GetAssetDirectory()
{
  static const std::string assetDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path assetDir;
    if (fs::exists(cwd / "Asset"))
    {
      assetDir = fs::absolute(cwd / "Asset");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Asset"))
      {
        assetDir = fs::absolute(cwd / "Asset");
      }
      else
      {
        abort();
      }
    }
    return assetDir.string();
  }();

  return assetDir.c_str();
}

static const char* GetLibraryDirectory() 
{
  static const std::string libraryDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path libraryDir;
    if (fs::exists(cwd / "Library"))
    {
      libraryDir = fs::absolute(cwd / "Library");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Library"))
      {
        libraryDir = fs::absolute(cwd / "Library");
      }
      else
      {
        abort();
      }
    }
    return libraryDir.string();
  }();

  return libraryDir.c_str();
}

static const char* GetResourceDirectory()
{
  static const std::string resourceDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path resourceDir;
    if (fs::exists(cwd / "Library"))
    {
      fs::create_directory(cwd / "Library" / "Resource");
      resourceDir = fs::absolute(cwd / "Library" / "Resource");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Library"))
      {
        fs::create_directory(cwd / "Library" / "Resource");
        resourceDir = fs::absolute(cwd / "Library" / "Resource");
      }
      else
      {
        abort();
      }
    }
    return resourceDir.string();
  }();

  return resourceDir.c_str();
}

static const char* GetFontDirectory()
{
  static const std::string fontDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path fontDir;
    if (fs::exists(cwd / "Library"))
    {
      fs::create_directory(cwd / "Library" / "Font");
      fontDir = fs::absolute(cwd / "Library" / "Font");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Library"))
      {
        fs::create_directory(cwd / "Library" / "Font");
        fontDir = fs::absolute(cwd / "Library" / "Font");
      }
      else
      {
        abort();
      }
    }
    return fontDir.string();
  }();

  return fontDir.c_str();
}

static const char* GetDataDirectory()
{
  static const std::string dataDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path dataDir;
    if (fs::exists(cwd / "Library"))
    {
      fs::create_directory(cwd / "Library" / "CSV");
      dataDir = fs::absolute(cwd / "Library" / "CSV");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Library"))
      {
        fs::create_directory(cwd / "Library" / "CSV");
        dataDir = fs::absolute(cwd / "Library" / "CSV");
      }
      else
      {
        abort();
      }
    }
    return dataDir.string();
  }();

  return dataDir.c_str();
}

static const char* GetShaderDirectory() {
    static const std::string shaderDir = []() -> std::string {
    fs::path cwd{fs::current_path()};
    fs::path shaderDir;
    if (fs::exists(cwd / "Library"))
    {
      fs::create_directory(cwd / "Library" / "Shader");
      shaderDir = fs::absolute(cwd / "Library" / "Shader");
    }
    else
    { // Go to the parent path.
      cwd = cwd.parent_path();
      if (fs::exists(cwd / "Library"))
      {
        fs::create_directory(cwd / "Library" / "Shader");
        shaderDir = fs::absolute(cwd / "Library" / "Shader");
      }
      else
      {
        abort();
      }
    }
    return shaderDir.string();
  }();

  return shaderDir.c_str();
}

// Extern definitions
const char* ns::kProjectDir = GetProjectDirectory();
const char* ns::kAssetDir = GetAssetDirectory();
const char* ns::kResourceDir = GetResourceDirectory();
const char* ns::kShaderDir = GetShaderDirectory();


const char* ns::kLibraryDir = GetLibraryDirectory();
const char* ns::kFontDir = GetFontDirectory();
const char* ns::kDataDir = GetDataDirectory();
