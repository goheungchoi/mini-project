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

// Extern definitions
const char* ns::kProjectDir = GetProjectDirectory();
const char* ns::kAssetDir = GetAssetDirectory();
const char* ns::kResourceDir = GetResourceDirectory();
