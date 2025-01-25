#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;
#include <unordered_map>

#include "assimp/GltfMaterial.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

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

  struct KeyPosition
  {
    float position[4];
    float timeStamp;
  };

  struct KeyRotation
  {
    /**
     * @brief Orientation angles represented by quaternion.
     * RotationAngle is in radians
     * x = RotationAxis.x * sin(RotationAngle / 2)
     * y = RotationAxis.y * sin(RotationAngle / 2)
     * z = RotationAxis.z * sin(RotationAngle / 2)
     * w = cos(RotationAngle / 2)
     *
     * To get the rotation angle,
     * std::acos(w) * 2.
     */
    float orientation[4];
    float timeStamp;
  };

  struct KeyScaling
  {
    float scaling[4];
    float timeStamp;
  };

  struct AnimationChannel
  {
    int boneId; // The id of the bone if it's a skeletal animation's key frame.
                // Otherwise, -1.
    std::string nodeName; // The name of the node (or bone) influenced by this
                          // key frame.

    uint32_t numKeyPositions;
    uint32_t numKeyRotations;
    uint32_t numKeyScalings;
    std::vector<KeyPosition> keyPositions;
    std::vector<KeyRotation> keyRotations;
    std::vector<KeyScaling> keyScalings;
  };

  struct Animation
  {
    std::string path;
    std::string name;

    float duration;
    float ticksPerSecond;

    aiMatrix4x4 globalInverseTransform;
    
		std::vector<AnimationChannel> animationChannels;
  };

  // Skeleton extraction
  static constexpr uint32_t kMaxBoneInfluences{8};

	using BoneId = int;
	using BoneWeight = float;

	struct VertexBoneWeight
  {
    BoneId boneId;
    BoneWeight weight;
  };

  struct Bone
  {
    BoneId id;
    std::string name;
    aiMatrix4x4 offset;
  };

  // Define a structure for skeleton nodes
  struct SkeletonNode
  {
    std::string name;
    aiMatrix4x4 transform;
    
		int level;
    int parent;
    int myIndex;
    int firstChild;
    int nextSibling;

    BoneId boneId;
  };

	struct Skeleton
  {
    std::string path;
    std::string name;

		std::unordered_map<std::string, BoneId> boneNameIdMap;
    std::vector<Bone> bones;
    std::vector<SkeletonNode> nodes;
	};
  

  // Mesh AABB
  struct AABB
  {
    float min[3];
    float max[3];
  };

  // Type definitions to store the model data
  enum AlphaMode : int16_t
  {
    AlphaMode_kOpaque = 0, // Alpha value is ignored
    AlphaMode_kMask = 1,   // Alpha cutoff
    AlphaMode_kBlend = 2   // Blended with the background
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
    std::string albedoTexture; // Path to the texture

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
  struct Mesh
  {
    std::string path;
    std::string name;

    AABB aabb;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::string materialPath;

    // Bone info
    std::vector<Bone> bones;
    std::vector<std::vector<VertexBoneWeight>> vertexBoneWeights;
  };

  // Geometry data
  struct GeometryNode
  {
    std::string name;
    aiMatrix4x4 transform;

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

    // Model bone data
    std::optional<std::string> skeletonPath;

		// Model Animation data
    std::unordered_map<std::string, Animation> animationPathMap;
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
      : assetDir(assetDir), resourceDir(resourceDir)
  {
  }

  /**
   * @brief Export a model.
   * @param path The full path to the model. Should starts such as "C:/...".
   * @param fileFormat
   * @return
   */
  bool ExportModel(const char* path, ModelFileFormat fileFormat,
                   bool preCalculateVertex = false, bool extractBones = false, bool exportAnimation = false);

private:
  std::unordered_set<std::string> _meshNameRegistry;
  std::unordered_set<std::string> _materialNameRegistry;
  bool _extractBones{false};
  bool _exportAnim{false};

  XMMATRIX _globalInverseTransform{};

  void ProcessScene(const aiScene* scene);
  void ProcessNode(GeometryModel& geoModel, GeometryNode& parentGeoNode,
                   aiNode* node, const aiScene* scene);
  void ProcessMesh(GeometryModel& geoModel, GeometryNode& geoNode, aiMesh* mesh,
                   const aiScene* scene);
  void ProcessMaterial(GeometryModel& geoModel, GeometryNode& geoNode,
                       Mesh& geoMesh, aiMaterial* material,
                       const aiScene* scene);
  void ProcessMaterialTexture(GeometryModel& geoModel, Material& geoMat,
                              aiMaterial* material, aiTextureType type,
                              const aiScene* scene);

  void ExportGeometryModel(GeometryModel& geoModel);
  void ExportModelMesh(Mesh& geoMesh);
  void ExportModelMaterial(Material& geoMat);
  void ExportModelTexture(Texture& texture);

	std::string GetUUID(std::string path);
  std::string GetExportPath(std::string path);

  void GenerateGeometryModelInfoFile(GeometryModel& geoModel);
  void GenerateModelMeshInfoFile(Mesh& geoMesh);
  void GenerateModelMaterialInfoFile(Material& geoMat);

private:
  Skeleton _skeleton;

  void ExtractSkeleton(const aiScene* scene);
  void ProcessSkeletonNode(Skeleton& skeleton, SkeletonNode& parentSkeletonNode,
                           aiNode* node, const aiScene* scene);

	void ExtractMeshBoneInfluences(Mesh& geoMesh, aiMesh* mesh,
                                 const aiScene* scene);

	void ExportSkeleton(Skeleton& skeleton);
  void GenerateSkeletonInfoFile(Skeleton& skeleton);

private:
  std::unordered_set<std::string> _animationNameRegistry;

  void ProcessAnimations(const aiScene* scene);
  void ProcessAnimation(Animation& geoAnim, const aiAnimation* anim,
                        const aiScene* scene);
  void ProcessAnimationChannel(AnimationChannel& animChannel,
                               const aiNodeAnim* channel, const aiScene* scene);


  void ExportAnimation(Animation& geoAnim);
  void GenerateAnimationInfoFile(Animation& geoAnim);

private:
  size_t GetMaxNodeCount(const aiScene* scene);
};
