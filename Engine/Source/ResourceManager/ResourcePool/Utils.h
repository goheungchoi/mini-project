#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include "Core/UUID/UUID.h"

extern const fs::path kResourceDir;

inline fs::path GetResourceDirectory() {
  fs::path cwd = fs::current_path();

	if (fs::exists(cwd/"Library/Resource"))
  {
    return cwd/"Library/Resource";
  }
  else	// Local debug mode
  {
    fs::path parent = cwd.parent_path();
    if (fs::exists(parent / "Library/Resource"))
    {
      return parent / "Library/Resource";
    }
    else
    {
      parent = parent.parent_path();
      if (fs::exists(parent / "Library/Resource"))
      {
        return parent / "Library/Resource";
      }
		}
	}

	throw std::exception("Can't locate the resource directory!");
  return fs::path();
}

inline fs::path GetResourcePath(xUUID uuid) {
	std::string strUUID = uuid.ToString();
	return kResourceDir / strUUID.substr(0, 2) / strUUID;
}
