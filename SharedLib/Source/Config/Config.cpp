#include "Shared/Config/Config.h"


#include <filesystem>
namespace fs = std::filesystem;

static const char* GetProjectDirectory()
{
  static const fs::path projectDir = []() -> fs::path {
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
    return projectDir;
  }();

  return (const char*)projectDir.c_str();
}

static const char* GetAssetDirectory()
{
  static const fs::path assetDir = []() -> fs::path {
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
    return assetDir;
  }();

  return (const char*)assetDir.c_str();
}

static const char* GetResourceDirectory()
{
  static const fs::path resourceDir = []() -> fs::path {
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
    return resourceDir;
  }();

  return (const char*)resourceDir.c_str();
}

// Extern definitions
const char* ns::kProjectDir = GetProjectDirectory();
const char* ns::kAssetDir = GetAssetDirectory();
const char* ns::kResourceDir = GetResourceDirectory();
