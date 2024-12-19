#pragma once

enum class ModelFileFormat
{
	kFBX,
	kOBJ,
	kGLTF
};

bool modelExport(const char* path, ModelFileFormat fileFormat);
