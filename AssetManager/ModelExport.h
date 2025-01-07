#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/GltfMaterial.h"

#include "TextureExport.h"

#include "dxgiformat.h"

#include "Shared/Serialize/model_generated.h"
#include <flatbuffers/flatbuffers.h>

enum class ModelFileFormat
{
	kUnknown,
	kFBX,
	kOBJ,
	kGLTF
};

class ModelExporter
{
	// TODO: Animation export

	// TODO: Skeleton extraction

	// TODO: Mesh AABB
  struct AABB
  {
    float min[3];
    float max[3];
	};

	// Type definitions to store the model data
  enum AlphaMode : int16_t
  {
    AlphaMode_kOpaque = 0,		// Alpha value is ignored
    AlphaMode_kMask = 1,			// Alpha cutoff
    AlphaMode_kBlend = 2			// Blended with the background
  };

	struct Vertex
  {
    float position[4];
    float normal[3];
    float tangent[3];
    float bitangent[3];
    float texcoord[2];
    float color[4];
  };

	// Texture
  struct Texture
  {
    std::string path;

		aiTextureType type;

		AlphaMode alphaMode;
	
		const aiTexture* embedded;
	};

	// Material
  struct Material
  {
    std::string path;
    std::string name;

		float albedoFactor[4];
    std::string albedoTexture;	// Path to the texture

		float metallicFactor;
    float roughnessFactor;
    std::string metallicRoughnessTexture;

		std::string normalTexture;

		std::string occlusionTexture;

		float emissiveFactor;
    std::string emissiveTexture;

		AlphaMode alphaMode;
    float alphaCutoff;
    bool doubleSided;
	};

	// Mesh
	struct Mesh {
    std::string path;
    std::string name;

		AABB aabb;
		std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

		std::string materialPath;
	};

	// Geometry data
  struct GeometryNode
  {
    std::string name;

		int level;
    int parent;
    int myIndex;
    int firstChild;
    int nextSibling;

		std::vector<std::string> meshPaths;
	};

	// Model
	struct GeometryModel
  {
    std::string path;
    std::string name;

		std::vector<GeometryNode> nodes;
    std::unordered_map<std::string, Mesh> meshPathMap;
    std::unordered_map<std::string, Material> materialPathMap;
    std::unordered_map<std::string, Texture> texturePathMap;
	};

	GeometryModel _geoModel;

private:
  fs::path _fullPath;
  fs::path _fullDirectory;

	// Path are relative to the asset directory and resource directory.
	// The resource directory is the mirror of the asset directory, but
	// name the assets by UUIDs.
  fs::path _path;
  fs::path _directory;

public:

	fs::path assetDir;
  fs::path resourceDir;

	ModelExporter(const fs::path& assetDir, const fs::path& resourceDir)
      : assetDir(assetDir), resourceDir(resourceDir) {}

  /**
   * @brief Export a model.
   * @param path The full path to the model. Should starts such as "C:/...".
   * @param fileFormat 
   * @return 
   */
  bool ExportModel(const char* path, ModelFileFormat fileFormat,
                   bool preCalculateVertex = false, bool extractBones = false);

private:
  void ProcessScene(const aiScene* scene);
  void ProcessNode(GeometryModel& geoModel, GeometryNode& parentGeoNode, aiNode* node, const aiScene* scene);
  void ProcessMesh(GeometryModel& geoModel, GeometryNode& geoNode, aiMesh* mesh,
                   const aiScene* scene);
  void ProcessMaterial(GeometryModel& geoModel, GeometryNode& geoNode,
                       Mesh& geoMesh, aiMaterial* material,
                       const aiScene* scene);
  void ProcessMaterialTexture(GeometryModel& geoModel, Material& geoMat, aiMaterial* material, aiTextureType type,
                                  const aiScene* scene);

	void ExportGeometryModel(GeometryModel& geoModel);
  void ExportModelMesh(Mesh& geoMesh);
  void ExportModelMaterial(Material& geoMat);
	void ExportModelTexture(Texture& texture);

	std::string GetExportPath(std::string path);
};
