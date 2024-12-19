#include "ModelExport.h"

#include <vector>
#include <string>
#include <filesystem>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "TextureExport.h"

struct Vertex
{
  float position[4];
  float normal[3];
  float tangent[3];
  float bitangent[3];
  float uv[2];
  float color[4];
};

static std::string _path;
static std::string _directory;

static void ProcessNode(aiNode* node, const aiScene* scene);
static void ProcessMesh(aiMesh* mesh, const aiScene* scene);

bool modelExport(const char* path, ModelFileFormat fileFormat)
{
  _path = path;

  Assimp::Importer importer;

	uint32_t flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded |
                   aiProcess_GenNormals | aiProcess_CalcTangentSpace;
  if (fileFormat == ModelFileFormat::kFBX)
  {
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
  }
  else if (fileFormat == ModelFileFormat::kGLTF)
  {
    flags = aiProcess_FlipUVs;
	}

	const aiScene* pScene = importer.ReadFile(path, flags);

	if (!pScene || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !pScene->mRootNode) {
    return false;
	}
  
	_directory = _path.substr(0, _path.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);

  return true;
}

void ProcessNode(aiNode* node, const aiScene* scene) {
  for (int i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[i];
    ProcessMesh(mesh, scene);
	}

  for (int i = 0; i < node->mNumChildren; ++i)
  {
    



		ProcessNode(node->mChildren[i], scene);
	}


}

void ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3);

	// Process vertices;
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex v{
        .position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0},
        .normal = {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        },
        .tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z},
        .bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z},
    };

		if (mesh->HasTextureCoords(i))
    {
      v.uv[0] = mesh->mTextureCoords[0][i].x;
      v.uv[1] = mesh->mTextureCoords[0][i].y;
    }
		
		if (mesh->HasVertexColors(i))
    {
      v.color[0] = mesh->mColors[0][i].r;
      v.color[1] = mesh->mColors[0][i].g;
      v.color[2] = mesh->mColors[0][i].b;
      v.color[3] = mesh->mColors[0][i].a;
		}

		vertices[i] = v;
	}

	// Process indices;
  std::size_t count = 0ULL;
  for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace face = mesh->mFaces[i];
    for (std::size_t j = 0; j < face.mNumIndices; ++j)
    {
      indices[count++] = face.mIndices[j];
    }
  }

	// Process Materials
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


	}

}


