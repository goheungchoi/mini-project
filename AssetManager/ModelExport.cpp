#include "ModelExport.h"

#include <stb_image.h>

#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
using namespace nlohmann;

#include "UUID.h"

// TODO: Animation data
// TODO: Skeletal mesh
bool ModelExporter::ExportModel(const char* path, ModelFileFormat fileFormat,
                                bool preCalculateVertex, bool extractBones,
                                bool exportAnimation)
{
  // Extract bone flag state change
  _extractBones = extractBones;
	// Export animation flag
  _exportAnim = exportAnimation;

  // Get the full path of the model and it's directory
  _fullPath = path;
  _fullDirectory = _fullPath.parent_path();
  if (fs::is_directory(_fullPath))
  {
    return false;
  }

  Assimp::Importer importer;

  uint32_t flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded |
                   aiProcess_GenNormals | aiProcess_CalcTangentSpace |
                   aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes;
  if (fileFormat == ModelFileFormat::kFBX)
  {
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
  }
  else if (fileFormat == ModelFileFormat::kGLTF)
  {
    flags |= aiProcess_FlipUVs;
  }

  // Pre-calculate vertex option
  if (preCalculateVertex)
  {
    flags |= aiProcess_PreTransformVertices;
  }

  const aiScene* pScene = importer.ReadFile(path, flags);

  if (!pScene || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !pScene->mRootNode)
  {
    return false;
  }

  // Relative path from the asset directory
  _path = fs::relative(_fullPath, fs::absolute(assetDir));
  // Relative directory
  _directory = _path.parent_path();

  // Get the geometry model path and the model name
  _geoModel.path = _path.string();
  _geoModel.name = pScene->mName.C_Str();
  // Reserve the space in the node vector
  size_t numMaxNodes = GetMaxNodeCount(pScene);
  _geoModel.nodes.reserve(numMaxNodes);

  // Extract bones option
  if (_extractBones)
  {
		_skeleton.nodes.reserve(numMaxNodes);
    ExtractSkeleton(pScene);
  }

  // Process the scene
  ProcessScene(pScene);

	// Process animation data
  if (_exportAnim)
  {
    if (!pScene->HasAnimations())
			return false;

		ProcessAnimations(pScene);
	}

  // Export the geometry model
  ExportGeometryModel(_geoModel);

  return true;
}

void ModelExporter::ProcessScene(const aiScene* scene)
{
  // Start from the root node and progress recursively.
  GeometryNode geoNode;
  geoNode.name = scene->mRootNode->mName.C_Str();
  geoNode.transform = scene->mRootNode->mTransformation;
  geoNode.level = 0;
  geoNode.parent = -1;
  geoNode.myIndex = 0;
  geoNode.firstChild = -1;
  geoNode.nextSibling = -1;
  _geoModel.nodes.push_back(geoNode);
  ProcessNode(_geoModel, _geoModel.nodes[0], scene->mRootNode, scene);
}

void ModelExporter::ProcessNode(GeometryModel& geoModel,
                                GeometryNode& parentGeoNode, aiNode* node,
                                const aiScene* scene)
{
  // Process the meshes of the current geo node.
  for (int i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    ProcessMesh(geoModel, parentGeoNode, mesh, scene);
  }

  // Store the child nodes
  for (int i = 0; i < node->mNumChildren; ++i)
  {
    if (i == 0)
    {
      parentGeoNode.firstChild = geoModel.nodes.size();
    }

    GeometryNode geoNode;
    geoNode.name = node->mChildren[i]->mName.C_Str();
    geoNode.transform = node->mTransformation;
    geoNode.level = parentGeoNode.level + 1;
    geoNode.parent = parentGeoNode.myIndex;
    geoNode.myIndex = geoModel.nodes.size();
    geoNode.firstChild = -1;
    geoNode.nextSibling =
        i < (node->mNumChildren - 1) ? geoNode.myIndex + 1 : -1;
    geoModel.nodes.push_back(geoNode);
  }

  // Process the child nodes
  for (int i = 0; i < node->mNumChildren; ++i)
  {
    int childIndex = parentGeoNode.firstChild + i;
    ProcessNode(geoModel, geoModel.nodes[childIndex], node->mChildren[i],
                scene);
  }
}

void ModelExporter::ProcessMesh(GeometryModel& geoModel, GeometryNode& geoNode,
                                aiMesh* mesh, const aiScene* scene)
{
  Mesh geoMesh;

  // Generate the unique name for the mesh
  unsigned int nameTag{0};
  std::string meshName = mesh->mName.C_Str() + std::to_string(nameTag);
  auto it = _meshNameRegistry.find(meshName);
  while (it != _meshNameRegistry.end())
  {
    ++nameTag;
    meshName = mesh->mName.C_Str() + std::to_string(nameTag);
    it = _meshNameRegistry.find(meshName);
  }
  _meshNameRegistry.insert(meshName);

  // Get the virtual path of the mesh
  // It should look something like _fullDirectory/mesh_name.mesh
  geoMesh.path = (_directory / meshName).string() + ".mesh";
  // Get name of the mesh
  geoMesh.name = meshName;

  // Get the AABB
  geoMesh.aabb =
      AABB{.min = {mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z},
           .max = {mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z}};

  // Get vertices and indices
  std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3);

  // Process vertices;
  for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex v{.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                          mesh->mVertices[i].z, 1.0},
             .normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                        mesh->mNormals[i].z}};

    if (mesh->HasTangentsAndBitangents())
    {
      v.tangent[0] = mesh->mTangents[i].x;
      v.tangent[1] = mesh->mTangents[i].y;
      v.tangent[2] = mesh->mTangents[i].z;

      v.bitangent[0] = mesh->mBitangents[i].x;
      v.bitangent[1] = mesh->mBitangents[i].y;
      v.bitangent[2] = mesh->mBitangents[i].z;
    }

    // Check if any texture coord set exists
    if (mesh->HasTextureCoords(0))
    {
      v.texcoord[0] = mesh->mTextureCoords[0][i].x;
      v.texcoord[1] = mesh->mTextureCoords[0][i].y;
    }

    // Check if any vertex color set exists
    if (mesh->HasVertexColors(0))
    {
      v.color[0] = mesh->mColors[0][i].r;
      v.color[1] = mesh->mColors[0][i].g;
      v.color[2] = mesh->mColors[0][i].b;
      v.color[3] = mesh->mColors[0][i].a;
    }

    vertices[i] = v;
  }

  // Process indices.
  std::size_t count = 0ULL;
  for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace face = mesh->mFaces[i];
    for (std::size_t j = 0; j < face.mNumIndices; ++j)
    {
      indices[count++] = face.mIndices[j];
    }
  }

  // Move the vertices and indices data to the mesh
  geoMesh.vertices = std::move(vertices);
  geoMesh.indices = std::move(indices);

	// Extract the bone influences in the mesh.
  if (_extractBones)
  {
    ExtractMeshBoneInfluences(geoMesh, mesh, scene);
  }

  // Process the mesh's material
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    ProcessMaterial(geoModel, geoNode, geoMesh, material, scene);
  }

  // Bind the mesh to the node
  geoNode.meshPaths.push_back(geoMesh.path);

  // Insert the mesh
  geoModel.meshPathMap[geoMesh.path] = std::move(geoMesh);
}

void ModelExporter::ProcessMaterial(GeometryModel& geoModel,
                                    GeometryNode& geoNode, Mesh& geoMesh,
                                    aiMaterial* material, const aiScene* scene)
{
  Material geoMat{};

  // Generate the unique name for the mesh
  unsigned int nameTag{0};
  std::string materialName =
      material->GetName().C_Str() + std::to_string(nameTag);
  auto it = _materialNameRegistry.find(materialName);
  while (it != _materialNameRegistry.end())
  {
    ++nameTag;
    materialName = material->GetName().C_Str() + std::to_string(nameTag);
    it = _materialNameRegistry.find(materialName);
  }
  _materialNameRegistry.insert(materialName);

  // Get the virtual path of the material.
  // It will be _directory/mat_name.mat
  geoMat.path = (_directory / materialName).string() + ".mat";

  // Get name
  geoMat.name = materialName;

  // Need alpha mode before getting the textures.
  // Get alpha mode
  aiString alphaMode;
  if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_ALPHAMODE, alphaMode))
  {
    if (alphaMode == aiString("OPAQUE"))
    {
      geoMat.alphaMode = AlphaMode_kOpaque;
    }
    else if (alphaMode == aiString("MASK"))
    {
      geoMat.alphaMode = AlphaMode_kMask;
    }
    else if (alphaMode == aiString("BLEND"))
    {
      geoMat.alphaMode = AlphaMode_kBlend;
    }
  }
  // Get alpha cutoff
  float alphaCutoff;
  if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_ALPHACUTOFF, alphaCutoff))
  {
    geoMat.alphaCutoff = alphaCutoff;
  }

  // Get double sided
  bool doubleSided;
  if (AI_SUCCESS == material->Get(AI_MATKEY_TWOSIDED, doubleSided))
  {
    geoMat.doubleSided = doubleSided;
  }

  // Get albedo factor
  aiColor4D albedoFactor{};
  if (AI_SUCCESS == material->Get(AI_MATKEY_BASE_COLOR, albedoFactor))
  {
    memcpy(geoMat.albedoFactor, &albedoFactor.r, 4 * sizeof(float));
  }

  // Get albedo texture
  int baseColorCount = material->GetTextureCount(aiTextureType_BASE_COLOR);
  if (baseColorCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material, aiTextureType_BASE_COLOR,
                           scene);
  }

  // Get metallic factor
  float metallicFactor;
  if (AI_SUCCESS == material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor))
  {
    geoMat.metallicFactor = metallicFactor;
  }
  // Get roughness factor
  float roughnessFactor;
  if (AI_SUCCESS == material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor))
  {
    geoMat.roughnessFactor = roughnessFactor;
  }

  // Get metallic roughness texture
  int matalicRoughnessCount = material->GetTextureCount(aiTextureType_UNKNOWN);
  if (matalicRoughnessCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material, aiTextureType_UNKNOWN,
                           scene);
  }

  // Get normal texture
  int normalTextureCount = material->GetTextureCount(aiTextureType_NORMALS);
  if (normalTextureCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material, aiTextureType_NORMALS,
                           scene);
  }

  // Get ambient occlusion texture
  int occlusionTextureCount =
      material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
  if (occlusionTextureCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material,
                           aiTextureType_AMBIENT_OCCLUSION, scene);
  }

  // Get emissive factor
  float emissiveFactor;
  if (AI_SUCCESS == material->Get(AI_MATKEY_EMISSIVE_INTENSITY, emissiveFactor))
  {
    geoMat.emissiveFactor = emissiveFactor;
  }
  // Get emissive texture
  int emissiveTextureCount =
      material->GetTextureCount(aiTextureType_EMISSION_COLOR);
  if (emissiveTextureCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material,
                           aiTextureType_EMISSION_COLOR, scene);
  }

  // Bind the material to the mesh
  geoMesh.materialPath = geoMat.path;

  // Insert the material data with no duplicate
  if (auto it = geoModel.materialPathMap.find(geoMat.path);
      it == geoModel.materialPathMap.end())
  {
    geoModel.materialPathMap.insert({geoMat.path, std::move(geoMat)});
  }
}

void ModelExporter::ProcessMaterialTexture(GeometryModel& geoModel,
                                           Material& geoMat,
                                           aiMaterial* material,
                                           aiTextureType type,
                                           const aiScene* scene)
{
  aiString path;
  material->GetTexture(type, 0, &path);

  Texture texture{
      .path = (_directory / path.C_Str()).string(),

      .type = type,

      .alphaMode = geoMat.alphaMode,

      .embedded = scene->GetEmbeddedTexture(path.C_Str()),
  };

  // Bind the texture to the geoMat
  if (type == aiTextureType_BASE_COLOR)
  {
    geoMat.albedoTexture = texture.path;
  }
  else if (type == aiTextureType_UNKNOWN)
  {
    geoMat.metallicRoughnessTexture = texture.path;
  }
  else if (type == aiTextureType_NORMALS)
  {
    geoMat.normalTexture = texture.path;
  }
  else if (type == aiTextureType_AMBIENT_OCCLUSION)
  {
    geoMat.occlusionTexture = texture.path;
  }
  else if (type == aiTextureType_EMISSION_COLOR)
  {
    geoMat.emissiveTexture = texture.path;
  }

  // Insert the texture data with no duplicate
  if (auto it = geoModel.texturePathMap.find(texture.path);
      it == geoModel.texturePathMap.end())
  {
    geoModel.texturePathMap.insert({texture.path, std::move(texture)});
  }
}

void ModelExporter::ExportGeometryModel(GeometryModel& geoModel)
{
  flatbuffers::FlatBufferBuilder builder;

	// Push back texture paths
  std::vector<flatbuffers::Offset<flatbuffers::String>> animationPaths;
  animationPaths.reserve(geoModel.animationPathMap.size());
  for (auto& [path, anim] : geoModel.animationPathMap)
  {
    animationPaths.push_back(builder.CreateString(path));
    ExportAnimation(anim);
  }

	// Push back the skeletal mesh's path
  flatbuffers::Offset<flatbuffers::String> skeletonPath = 0;
  if (geoModel.skeletonPath)
  {
    skeletonPath = builder.CreateString(geoModel.skeletonPath.value());
    ExportSkeleton(_skeleton);
	}

  // Push back texture paths
  std::vector<flatbuffers::Offset<flatbuffers::String>> texturePaths;
  texturePaths.reserve(geoModel.texturePathMap.size());
  for (auto& [path, texture] : geoModel.texturePathMap)
  {
    texturePaths.push_back(builder.CreateString(path));
    ExportModelTexture(texture);
  }

  // Push back material paths
  std::vector<flatbuffers::Offset<flatbuffers::String>> materialPaths;
  materialPaths.reserve(geoModel.materialPathMap.size());
  for (auto& [path, material] : geoModel.materialPathMap)
  {
    materialPaths.push_back(builder.CreateString(path));
    ExportModelMaterial(material);
  }

  // Push back mesh paths
  std::vector<flatbuffers::Offset<flatbuffers::String>> meshPaths;
  meshPaths.reserve(geoModel.meshPathMap.size());
  for (auto& [path, mesh] : geoModel.meshPathMap)
  {
    meshPaths.push_back(builder.CreateString(path));
    ExportModelMesh(mesh);
  }

  // Start building geometry nodes
  std::vector<flatbuffers::Offset<GameResource::GeometryNode>> nodesVector;
  nodesVector.reserve(geoModel.nodes.size());
  for (auto& node : geoModel.nodes)
  {
    auto name = builder.CreateString(node.name);

		auto transform = GameResource::Matrix(
        node.transform[0][0], node.transform[0][1], node.transform[0][2], node.transform[0][3], 
				node.transform[1][0], node.transform[1][1], node.transform[1][2], node.transform[1][3], 
				node.transform[2][0], node.transform[2][1], node.transform[2][2], node.transform[2][3], 
        node.transform[3][0], node.transform[3][1], node.transform[3][2], node.transform[3][3]);

    std::vector<flatbuffers::Offset<flatbuffers::String>> meshesVector;
    meshesVector.reserve(node.meshPaths.size());
    for (auto& meshPath : node.meshPaths)
    {
      meshesVector.push_back(builder.CreateString(meshPath));
    }

    auto meshes = builder.CreateVector(meshesVector);

    auto geometryNode = GameResource::CreateGeometryNode(
        builder, name, &transform, node.level, node.parent, node.firstChild,
        node.nextSibling, meshes);
    nodesVector.push_back(geometryNode);
  }

  auto name = builder.CreateString(geoModel.name);
  auto nodes = builder.CreateVector(nodesVector);
  auto meshes = builder.CreateVector(meshPaths);
  auto materials = builder.CreateVector(materialPaths);
  auto textures = builder.CreateVector(texturePaths);
  auto animations = builder.CreateVector(animationPaths);

  auto model =
      GameResource::CreateGeometryModel(builder, name, nodes, meshes, materials,
                                        textures, skeletonPath, animations);

  builder.Finish(model);

  // Write to file
  std::string exportPath = GetExportPath(geoModel.path);
  std::ofstream ofs(exportPath, std::ios::binary);
  ofs.write((char*)builder.GetBufferPointer(), builder.GetSize());
  ofs.close();

  // Generate the model info file
  GenerateGeometryModelInfoFile(geoModel);
}

void ModelExporter::ExportModelMesh(Mesh& geoMesh)
{
  flatbuffers::FlatBufferBuilder builder;

  auto name = builder.CreateString(geoMesh.name);

  auto aabb_min = GameResource::Vec3(geoMesh.aabb.min[0], geoMesh.aabb.min[1],
                                     geoMesh.aabb.min[2]);
  auto aabb_max = GameResource::Vec3(geoMesh.aabb.max[0], geoMesh.aabb.max[1],
                                     geoMesh.aabb.max[2]);
  auto aabb = GameResource::AABB(aabb_min, aabb_max);

  std::vector<GameResource::Vertex> verticesVector(
      geoMesh.vertices.size());
  for (size_t i = 0; i < geoMesh.vertices.size(); ++i)
  {
    auto& v = geoMesh.vertices[i];

    auto position = GameResource::Vec4(v.position[0], v.position[1],
                                       v.position[2], v.position[3]);
    auto normal = GameResource::Vec3(v.normal[0], v.normal[1],
                                           v.normal[2]);
    auto tangent = GameResource::Vec3(v.tangent[0], v.tangent[1],
                                            v.tangent[2]);
    auto bitangent = GameResource::Vec3(v.bitangent[0],
                                              v.bitangent[1], v.bitangent[2]);
    auto texcoord =
        GameResource::TexCoord(v.texcoord[0], v.texcoord[1]);
    auto color = GameResource::Vec4(v.color[0], v.color[1],
                                          v.color[2], v.color[3]);
    auto vertex = GameResource::Vertex(position, normal, tangent,
                                             bitangent, texcoord, color);
    verticesVector[i] = vertex;
  }

	std::vector<flatbuffers::Offset<GameResource::Bone>> bonesVector;
  bonesVector.reserve(geoMesh.bones.size());
  for (const Bone& bone : geoMesh.bones)
  {
    auto bone_name = builder.CreateString(bone.name);

    auto offset = GameResource::Matrix(
        bone.offset[0][0], bone.offset[0][1], bone.offset[0][2],
        bone.offset[0][3], bone.offset[1][0], bone.offset[1][1],
        bone.offset[1][2], bone.offset[1][3], bone.offset[2][0],
        bone.offset[2][1], bone.offset[2][2], bone.offset[2][3],
        bone.offset[3][0], bone.offset[3][1], bone.offset[3][2],
        bone.offset[3][3]);

    auto bone_offset =
        GameResource::CreateBone(builder, bone.id, bone_name, &offset);
    bonesVector.push_back(bone_offset);
  }

	size_t totalBoneWeightNum{geoMesh.vertexBoneWeights.size() *
                            kMaxBoneInfluences};
	std::vector<GameResource::VertexBoneWeight> boneWeightsVector;
  boneWeightsVector.resize(totalBoneWeightNum, {-1, 0});
  size_t index{0};
  for (const auto& vertexBoneWeight : geoMesh.vertexBoneWeights)
  {
    for (size_t i = 0; i < vertexBoneWeight.size(); ++i)
    {
      const auto& boneWeight = vertexBoneWeight[i];
      boneWeightsVector[index + i] =
          GameResource::VertexBoneWeight{(int32_t)boneWeight.boneId, boneWeight.weight};
		}
    index += kMaxBoneInfluences;
	}

  auto vertices = builder.CreateVectorOfStructs(verticesVector);
  auto indices = builder.CreateVector(geoMesh.indices);
  auto material = builder.CreateString(geoMesh.materialPath);
  auto bones = builder.CreateVector(bonesVector);
  auto boneWeigths = builder.CreateVectorOfStructs(boneWeightsVector);

  auto mesh = GameResource::CreateMesh(builder, name, &aabb, vertices, indices,
                                       material, bones, boneWeigths);

  builder.Finish(mesh);

  // Write to file
  std::string exportPath = GetExportPath(geoMesh.path);
  std::ofstream ofs(exportPath, std::ios::binary);
  ofs.write((char*)builder.GetBufferPointer(), builder.GetSize());
  ofs.close();

  // Generate the model mesh info file
  GenerateModelMeshInfoFile(geoMesh);
}

void ModelExporter::ExportModelMaterial(Material& geoMat)
{
  flatbuffers::FlatBufferBuilder builder;

  auto name = builder.CreateString(geoMat.name);

  auto albedoFactor = GameResource::Vec4(geoMat.albedoFactor[0], geoMat.albedoFactor[1],
      geoMat.albedoFactor[2], geoMat.albedoFactor[3]);
  auto albedoTexture = builder.CreateString(geoMat.albedoTexture);

  auto metallicRoughnessTexture =
      builder.CreateString(geoMat.metallicRoughnessTexture);
  auto normalTexture = builder.CreateString(geoMat.normalTexture);
  auto occlusionTexture = builder.CreateString(geoMat.occlusionTexture);
  auto emissiveTexture = builder.CreateString(geoMat.emissiveTexture);

  auto material = GameResource::CreateMaterial(
      builder, name, &albedoFactor, albedoTexture, geoMat.metallicFactor,
      geoMat.roughnessFactor, metallicRoughnessTexture, normalTexture,
      occlusionTexture, geoMat.emissiveFactor, emissiveTexture,
      (GameResource::AlphaMode)geoMat.alphaMode, geoMat.alphaCutoff,
      geoMat.doubleSided);

  builder.Finish(material);

  // Write to file
  std::string exportPath = GetExportPath(geoMat.path);
  std::ofstream ofs(exportPath, std::ios::binary);
  ofs.write((char*)builder.GetBufferPointer(), builder.GetSize());
  ofs.close();

  // Generate the model mesh info file
  GenerateModelMaterialInfoFile(geoMat);
}

void ModelExporter::ExportModelTexture(Texture& texture)
{
  ImageData data{};
  TextureExportOptions options{};
  if (texture.type == aiTextureType_BASE_COLOR)
  {
    data.isNormalMap = false;
    data.isCubeMap = false;

    data.colorSpace = ColorSpace::kSRGB;
    data.alphaMode = texture.alphaMode == AlphaMode_kBlend
                         ? nvtt::AlphaMode_Transparency
                         : nvtt::AlphaMode_None;

    options.format = ImageFormat::BC7;
    options.quality = nvtt::Quality_Normal;

    options.enableGammaCorrect = true;
    options.enablePremultipliedAlphaBlending =
        texture.alphaMode == AlphaMode_kBlend;
    options.enableAlphaDithering = false;
    options.enableMipmap = true;
    options.maxMipmapCount = 0;
    options.minMipmapSize = 1;
    options.mipmapFilter = nvtt::MipmapFilter_Box;
    options.useGPU = true;
  }
  else if (texture.type == aiTextureType_UNKNOWN)
  {
    // TODO: ARGB -> RGBA
    data.isNormalMap = false;
    data.isCubeMap = false;

    data.colorSpace = ColorSpace::kLinear;
    data.alphaMode = nvtt::AlphaMode_None;

    options.format = ImageFormat::BC5u;
    options.quality = nvtt::Quality_Normal;

    options.enableGammaCorrect = false;
    options.enablePremultipliedAlphaBlending = false;
    options.enableAlphaDithering = false;
    options.enableMipmap = true;
    options.maxMipmapCount = 0;
    options.minMipmapSize = 1;
    options.mipmapFilter = nvtt::MipmapFilter_Box;

    options.swizzleChannels = true;
    options.swizzles[0] = nvtt::Blue;
    options.swizzles[1] = nvtt::Green;
    options.swizzles[2] = nvtt::Red;
    options.swizzles[3] = nvtt::Zero;

    options.useGPU = true;
  }
  else if (texture.type == aiTextureType_NORMALS)
  {
    data.isNormalMap = true;
    data.isCubeMap = false;

    data.colorSpace = ColorSpace::kLinear;
    data.alphaMode = nvtt::AlphaMode_None;

    options.format = ImageFormat::BC7;
    options.quality = nvtt::Quality_Normal;

    options.enableGammaCorrect = false;
    options.enablePremultipliedAlphaBlending = false;
    options.enableAlphaDithering = false;
    options.enableMipmap = true;
    options.maxMipmapCount = 0;
    options.minMipmapSize = 1;
    options.mipmapFilter = nvtt::MipmapFilter_Box;
    options.useGPU = true;
  }
  else if (texture.type == aiTextureType_AMBIENT_OCCLUSION)
  {
    data.isNormalMap = false;
    data.isCubeMap = false;

    data.colorSpace = ColorSpace::kLinear;
    data.alphaMode = nvtt::AlphaMode_None;

    options.format = ImageFormat::BC4u;
    options.quality = nvtt::Quality_Normal;

    options.enableGammaCorrect = false;
    options.enablePremultipliedAlphaBlending = false;
    options.enableAlphaDithering = false;
    options.enableMipmap = true;
    options.maxMipmapCount = 0;
    options.minMipmapSize = 1;
    options.mipmapFilter = nvtt::MipmapFilter_Box;
    options.useGPU = true;
  }
  else if (texture.type == aiTextureType_EMISSION_COLOR)
  {
    data.isNormalMap = false;
    data.isCubeMap = false;

    data.colorSpace = ColorSpace::kSRGB;
    data.alphaMode = nvtt::AlphaMode_None;

    options.format = ImageFormat::BC7;
    options.quality = nvtt::Quality_Normal;

    options.enableGammaCorrect = true;
    options.enablePremultipliedAlphaBlending = false;
    options.enableAlphaDithering = false;
    options.enableMipmap = true;
    options.maxMipmapCount = 0;
    options.minMipmapSize = 1;
    options.mipmapFilter = nvtt::MipmapFilter_Box;
    options.useGPU = true;
  }

  // Get the export path from the texture path
  std::string exportPath = GetExportPath(texture.path);

  if (texture.embedded)
  {
    size_t size =
        texture.embedded->mHeight == 0
            ? texture.embedded->mWidth
            : texture.embedded->mWidth * texture.embedded->mHeight * 4;

    std::vector<char> input(size);
    memcpy(input.data(), texture.embedded->pcData, size);

    exportTextureFromMemory(input, texture.path, exportPath, data, options);
  }
  else
  {
    exportTextureFromFile((fs::absolute(assetDir) / texture.path).string(),
                          exportPath, data, options);
  }
}

std::string ModelExporter::GetUUID(std::string path)
{
  std::replace(path.begin(), path.end(), '/', '\\');
  return GenerateUUIDFromName(path).ToString();
}

std::string ModelExporter::GetExportPath(std::string path)
{
  std::string strUUID = GetUUID(path);

  fs::path resourceSubDir = fs::absolute(resourceDir) / strUUID.substr(0, 2);

  if (!fs::exists(resourceSubDir))
  {
    fs::create_directories(resourceSubDir);
  }

  fs::path exportPath = resourceSubDir / strUUID;

  return exportPath.string();
}

void ModelExporter::GenerateGeometryModelInfoFile(GeometryModel& geoModel)
{
  ordered_json j;

  j["uuid"] = GetUUID(geoModel.path);
  j["import_path"] = geoModel.path;
  j["resource_type"] = "model";

  ordered_json details;
  details["name"] = geoModel.name;

  std::vector<ordered_json> nodes;
  nodes.reserve(geoModel.nodes.size());
  for (const auto& node : geoModel.nodes)
  {
    ordered_json n;

    n["name"] = node.name;
    n["idx"] = node.myIndex;
    n["level"] = node.level;
    n["parent"] = node.parent;
    n["first_child"] = node.firstChild;
    n["next_sibling"] = node.nextSibling;
    n["meshes"] = node.meshPaths;

    nodes.push_back(n);
  }
  details["nodes"] = nodes;

  std::vector<std::reference_wrapper<const std::string>> meshes;
  meshes.reserve(geoModel.meshPathMap.size());
  for (const auto& [path, mesh] : geoModel.meshPathMap)
  {
    meshes.push_back(std::ref(path));
  }
  details["meshes"] = meshes;

  std::vector<std::reference_wrapper<const std::string>> materials;
  materials.reserve(geoModel.materialPathMap.size());
  for (const auto& [path, material] : geoModel.materialPathMap)
  {
    materials.push_back(std::ref(path));
  }
  details["materials"] = materials;

  std::vector<std::reference_wrapper<const std::string>> textures;
  textures.reserve(geoModel.texturePathMap.size());
  for (const auto& [path, texture] : geoModel.texturePathMap)
  {
    textures.push_back(std::ref(path));
  }
  details["textures"] = textures;

	if (geoModel.skeletonPath)
  {
    details["skeleton"] = geoModel.skeletonPath.value();
	}

  j["details"] = details;

  std::ofstream o(GetExportPath(geoModel.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::GenerateModelMeshInfoFile(Mesh& geoMesh)
{
  ordered_json j;

	j["uuid"] = GetUUID(geoMesh.path);
  j["import_path"] = geoMesh.path;
  j["resource_type"] = "model";

  ordered_json details;
  details["name"] = geoMesh.name;

  details["bounding_box"] = {
      {geoMesh.aabb.min[0], geoMesh.aabb.min[1], geoMesh.aabb.min[2]},
      {geoMesh.aabb.max[0], geoMesh.aabb.max[1], geoMesh.aabb.max[2]}};

  details["num_vertices"] = geoMesh.vertices.size();
  details["num_indices"] = geoMesh.indices.size();

  details["material"] = geoMesh.materialPath;

	std::vector<ordered_json> bones;
  bones.reserve(geoMesh.bones.size());
  for (const auto& bone : geoMesh.bones)
  {
    ordered_json b;

    b["id"] = bone.id;
    b["name"] = bone.name;
    b["inverse_bind_mat"] = {
        bone.offset[0][0], bone.offset[0][1], bone.offset[0][2],
        bone.offset[0][3], bone.offset[1][0], bone.offset[1][1],
        bone.offset[1][2], bone.offset[1][3], bone.offset[2][0],
        bone.offset[2][1], bone.offset[2][2], bone.offset[2][3],
        bone.offset[3][0], bone.offset[3][1], bone.offset[3][2],
        bone.offset[3][3]};

    bones.push_back(b);
  }
  details["bones"] = bones;

	std::vector<ordered_json> boneWeightsPerVertex;
  boneWeightsPerVertex.resize(geoMesh.vertexBoneWeights.size(),
                              ordered_json::array());
  size_t index{0};
  for (const auto& vertexBoneWeight : geoMesh.vertexBoneWeights)
  {
    for (size_t i = 0; i < vertexBoneWeight.size(); ++i)
    {
      const auto& boneWeight = vertexBoneWeight[i];
      ordered_json pair;
      pair["id"] = boneWeight.boneId;
      pair["weight"] = boneWeight.weight;
      boneWeightsPerVertex[index].push_back(pair);
    }
    index++;
  }
  details["bone_weights"] = boneWeightsPerVertex;

  j["details"] = details;

  std::ofstream o(GetExportPath(geoMesh.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::GenerateModelMaterialInfoFile(Material& geoMat)
{
  ordered_json j;

	j["uuid"] = GetUUID(geoMat.path);
  j["import_path"] = geoMat.path;
  j["resource_type"] = "model";

  ordered_json details;
  details["name"] = geoMat.name;

  details["albedo_factor"] = {{geoMat.albedoFactor[0], geoMat.albedoFactor[1],
                               geoMat.albedoFactor[2], geoMat.albedoFactor[3]}};
  details["albedo_texture"] = geoMat.albedoTexture;

  details["metallic_factor"] = geoMat.metallicFactor;
  details["roughness_factor"] = geoMat.roughnessFactor;
  details["metallic_roughness_texture"] = geoMat.metallicRoughnessTexture;

  details["normal_texture"] = geoMat.normalTexture;

  details["occlusion_texture"] = geoMat.occlusionTexture;

  details["emissive_factor"] = geoMat.emissiveFactor;
  details["emissive_texture"] = geoMat.emissiveTexture;

  details["alpha_mode"] = magic_enum::enum_name(geoMat.alphaMode);
  details["alpha_cutoff"] = geoMat.alphaCutoff;
  details["double_sided"] = geoMat.doubleSided;

  j["details"] = details;

  std::ofstream o(GetExportPath(geoMat.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::ExtractSkeleton(const aiScene* scene) {
  // Generate the name of the skeleton
  std::string skeletonName = scene->mRootNode->mName.C_Str();

  // Get the virtual path of the skeleton.
  // It will be _directory/skeleton_name.skel
  _skeleton.path = (_directory / skeletonName).string() + ".skel";
  _skeleton.name = skeletonName;

	// Bind it to the model
  _geoModel.skeletonPath = _skeleton.path;

  // Start from the root node and progress recursively.
  SkeletonNode skeletonNode;
  skeletonNode.name = scene->mRootNode->mName.C_Str();
  skeletonNode.transform = aiMatrix4x4();
  skeletonNode.level = 0;
  skeletonNode.parent = -1;
  skeletonNode.myIndex = 0;
  skeletonNode.firstChild = -1;
  skeletonNode.nextSibling = -1;
  skeletonNode.boneId = -1;
  _skeleton.nodes.push_back(skeletonNode);
  ProcessSkeletonNode(_skeleton, _skeleton.nodes[0], scene->mRootNode,
                      scene);
}

void ModelExporter::ProcessSkeletonNode(Skeleton& skeleton,
                                        SkeletonNode& parentSkeletonNode,
                                        aiNode* node, const aiScene* scene)
{
  //// Exclude the mesh nodes
  //if (node->mNumMeshes > 0)
  //  return;

  // Store skeleton nodes
  int count = 0;
  for (int i = 0; i < node->mNumChildren; ++i)
  {
    // Exclude the mesh nodes
    if (node->mChildren[i]->mNumMeshes > 0)
      continue;

    if (count == 0)
    {
      parentSkeletonNode.firstChild = skeleton.nodes.size();
    }

		// Create a skeleton node.
    SkeletonNode skeletonNode;
    skeletonNode.name = node->mChildren[i]->mName.C_Str();
    skeletonNode.transform = node->mChildren[i]->mTransformation;
    skeletonNode.level = parentSkeletonNode.level + 1;
    skeletonNode.parent = parentSkeletonNode.myIndex;
    skeletonNode.myIndex = skeleton.nodes.size();
    skeletonNode.firstChild = -1;
    skeletonNode.nextSibling = skeletonNode.myIndex + 1;
    

		// Create a bone.
		Bone bone;
    bone.id = skeleton.bones.size();
    bone.name = node->mChildren[i]->mName.C_Str();

		// Set the skeleton node's bone id.
    skeletonNode.boneId = bone.id;

		skeleton.nodes.push_back(skeletonNode);
    skeleton.bones.push_back(bone);

		// Map the name of the node and the bone id
    skeleton.boneNameIdMap[skeletonNode.name] = skeletonNode.boneId;

    ++count;
  }

	if (count > 0)
		skeleton.nodes.back().nextSibling = -1;

  // Process the child nodes
  count = 0;
  for (int i = 0; i < node->mNumChildren; ++i)
  {
    // Exclude the mesh nodes
    if (node->mChildren[i]->mNumMeshes > 0)
      continue;

    int childIndex = parentSkeletonNode.firstChild + count;
    ProcessSkeletonNode(skeleton, skeleton.nodes[childIndex],
                        node->mChildren[i], scene);
    ++count;
  }
}

void ModelExporter::ExtractMeshBoneInfluences(Mesh& geoMesh, aiMesh* mesh,
                                              const aiScene* scene)
{
	// Get the number of bones.
  const uint32_t numBones = mesh->mNumBones;
	if (numBones == 0)
    return;

	// Reserve the space to store the bone data
	geoMesh.bones.resize(numBones);
  geoMesh.vertexBoneWeights.resize(geoMesh.vertices.size());
  for (auto& weightVector : geoMesh.vertexBoneWeights)
  {
    weightVector.reserve(kMaxBoneInfluences);
	}

  // Process vertices.
  for (uint32_t boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
  {
    aiBone* currBone = mesh->mBones[boneIdx];

		const std::string boneName{currBone->mName.C_Str()};
		const uint32_t boneId = _skeleton.boneNameIdMap[boneName];
		
		// Retrieve the bone info
		Bone& bone = _skeleton.bones[boneId];
		bone.offset = currBone->mOffsetMatrix;
    geoMesh.bones[boneIdx] = bone;

		// Retrieve the influences of the bone to vertices
    uint32_t numWeights = std::min(currBone->mNumWeights, kMaxBoneInfluences);
		auto* vertexWeights = currBone->mWeights;
    for (uint32_t weightIdx = 0; weightIdx < numWeights; ++weightIdx)
    {
      uint32_t vertexId = vertexWeights[weightIdx].mVertexId;
      float weight = vertexWeights[weightIdx].mWeight;
      geoMesh.vertexBoneWeights[vertexId].push_back({bone.id, weight});
		}
  }
}

void ModelExporter::ExportSkeleton(Skeleton& skeleton) {
  flatbuffers::FlatBufferBuilder builder;

  auto name = builder.CreateString(skeleton.name);

	std::vector<flatbuffers::Offset<GameResource::Bone>> bones;
  bones.reserve(skeleton.bones.size());
  for (const Bone& bone : skeleton.bones)
  {
    auto bone_name = builder.CreateString(bone.name);

		auto offset = GameResource::Matrix(
        bone.offset[0][0], bone.offset[0][1], bone.offset[0][2], bone.offset[0][3], 
				bone.offset[1][0], bone.offset[1][1], bone.offset[1][2], bone.offset[1][3], 
				bone.offset[2][0], bone.offset[2][1], bone.offset[2][2], bone.offset[2][3],
        bone.offset[3][0], bone.offset[3][1], bone.offset[3][2], bone.offset[3][3]);

		auto bone_offset =
        GameResource::CreateBone(builder, bone.id, bone_name, &offset);
    bones.push_back(bone_offset);
	}

	std::vector<flatbuffers::Offset<GameResource::SkeletonNode>> nodes;
  nodes.reserve(skeleton.nodes.size());
  for (const SkeletonNode& node : skeleton.nodes)
  {
    auto node_name = builder.CreateString(node.name);

		auto node_transform = GameResource::Matrix(
        node.transform[0][0], node.transform[0][1], node.transform[0][2], node.transform[0][3], 
				node.transform[1][0], node.transform[1][1], node.transform[1][2], node.transform[1][3], 
				node.transform[2][0], node.transform[2][1], node.transform[2][2], node.transform[2][3],
        node.transform[3][0], node.transform[3][1], node.transform[3][2], node.transform[3][3]);

		auto node_offset = GameResource::CreateSkeletonNode(
        builder, node_name, &node_transform, node.level, node.parent,
        node.firstChild, node.nextSibling, node.boneId);

		nodes.push_back(node_offset);
  }
	
  auto bones_offset = builder.CreateVector(bones);
  auto nodes_offset = builder.CreateVector(nodes);

  auto skeleton_offset = GameResource::CreateSkeleton(builder, name, bones_offset, nodes_offset);

  builder.Finish(skeleton_offset);

  // Write to file
  std::string exportPath = GetExportPath(skeleton.path);
  std::ofstream ofs(exportPath, std::ios::binary);
  ofs.write((char*)builder.GetBufferPointer(), builder.GetSize());
  ofs.close();

	// Generate the skeleton info file
  GenerateSkeletonInfoFile(skeleton);
}

void ModelExporter::GenerateSkeletonInfoFile(Skeleton& skeleton) {
  ordered_json j;

	j["uuid"] = GetUUID(skeleton.path);
  j["import_path"] = skeleton.path;
  j["resource_type"] = "skeleton";

  ordered_json details;
  details["name"] = skeleton.name;

  std::vector<ordered_json> bones;
  bones.reserve(skeleton.bones.size());
  for (const auto& bone : skeleton.bones)
  {
    ordered_json b;

    b["id"] = bone.id;
    b["name"] = bone.name;
    b["inverse_bind_mat"] = {
        bone.offset[0][0], bone.offset[0][1], bone.offset[0][2], bone.offset[0][3], 
				bone.offset[1][0], bone.offset[1][1], bone.offset[1][2], bone.offset[1][3], 
				bone.offset[2][0], bone.offset[2][1], bone.offset[2][2], bone.offset[2][3],
        bone.offset[3][0], bone.offset[3][1], bone.offset[3][2], bone.offset[3][3]};

    bones.push_back(b);
  }
  details["bones"] = bones;

	std::vector<ordered_json> nodes;
  nodes.reserve(skeleton.nodes.size());
  for (const auto& node : skeleton.nodes)
  {
    ordered_json n;

    n["name"] = node.name;
    n["idx"] = node.myIndex;
    n["level"] = node.level;
    n["parent"] = node.parent;
    n["first_child"] = node.firstChild;
    n["next_sibling"] = node.nextSibling;
    n["boneId"] = node.boneId;

    nodes.push_back(n);
  }
  details["nodes"] = nodes;

  j["details"] = details;

  std::ofstream o(GetExportPath(skeleton.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::ProcessAnimations(const aiScene* scene) {
	
	for (uint32_t i = 0; i < scene->mNumAnimations; ++i)
  {
    Animation geoAnim;
    ProcessAnimation(geoAnim, scene->mAnimations[i], scene);
    // Insert the animation
    _geoModel.animationPathMap[geoAnim.path] = std::move(geoAnim);
  }
}

void ModelExporter::ProcessAnimation(Animation& geoAnim,
                                     const aiAnimation* anim,
                                     const aiScene* scene)
{
  // Generate the unique name for the animation
  std::string animationName = anim->mName.C_Str();
  auto it = _animationNameRegistry.find(animationName);

	unsigned int nameTag{1};
  while (animationName.empty() || it != _animationNameRegistry.end())
  {
    animationName = anim->mName.C_Str() + std::to_string(nameTag);
    it = _animationNameRegistry.find(animationName);
    ++nameTag;
  }
  _animationNameRegistry.insert(animationName);

	// Get the virtual path of the animation.
  // It will be _directory/mat_name.mat
  geoAnim.path = (_directory / animationName).string() + ".anim";

  // Get name
  geoAnim.name = animationName;

	// Get animation info
  geoAnim.duration = anim->mDuration;
  geoAnim.ticksPerSecond = anim->mTicksPerSecond;

	geoAnim.globalInverseTransform = scene->mRootNode->mTransformation;
  geoAnim.globalInverseTransform.Inverse();

	// Get animation channels
  geoAnim.animationChannels.reserve(anim->mNumChannels);
  for (int i = 0; i < anim->mNumChannels; ++i)
  {
    AnimationChannel channel;
    ProcessAnimationChannel(channel, anim->mChannels[i]);
    geoAnim.animationChannels.push_back(std::move(channel));
	}
}

void ModelExporter::ProcessAnimationChannel(AnimationChannel& animChannel,
                                            const aiNodeAnim* channel)
{
	// Get bone id if exists
  animChannel.boneId = -1;
  if (auto it = _skeleton.boneNameIdMap.find(channel->mNodeName.C_Str());
      it != _skeleton.boneNameIdMap.end())
  {
    animChannel.boneId = it->second;
	}
	// Get the name of the node of this channel
	animChannel.nodeName = channel->mNodeName.C_Str();

	// Read positions
  animChannel.numKeyPositions = channel->mNumPositionKeys;
  animChannel.keyPositions.resize(animChannel.numKeyPositions);
  for (int posIdx = 0; posIdx < animChannel.numKeyPositions; ++posIdx)
  {
    aiVector3D aiPosition = channel->mPositionKeys[posIdx].mValue;
    float timeStamp = channel->mPositionKeys[posIdx].mTime;

		KeyPosition pos{.position = {aiPosition.x, aiPosition.y, aiPosition.z, 1.f},
                    .timeStamp = timeStamp};

    animChannel.keyPositions[posIdx] = pos;
  }

  // Read rotations
  animChannel.numKeyRotations = channel->mNumRotationKeys;
  animChannel.keyRotations.resize(animChannel.numKeyRotations);
  for (int rotIdx = 0; rotIdx < animChannel.numKeyRotations; ++rotIdx)
  {
    aiQuaternion aiOrientation = channel->mRotationKeys[rotIdx].mValue;
    aiOrientation.Normalize();
    float timeStamp = channel->mRotationKeys[rotIdx].mTime;

		KeyRotation rot{.orientation = {aiOrientation.x, aiOrientation.y,
                                    aiOrientation.z, aiOrientation.w},
                    .timeStamp = timeStamp};

    animChannel.keyRotations[rotIdx] = rot;
  }

  // Read scales
  animChannel.numKeyScalings = channel->mNumScalingKeys;
  animChannel.keyScalings.resize(animChannel.numKeyScalings);
  for (int scaleIdx = 0; scaleIdx < animChannel.numKeyScalings;
       ++scaleIdx)
  {
    aiVector3D aiScale = channel->mScalingKeys[scaleIdx].mValue;
    float timeStamp = channel->mScalingKeys[scaleIdx].mTime;

		KeyScaling scale{.scaling = {aiScale.x, aiScale.y, aiScale.z, 0.f},
                     .timeStamp = timeStamp};

    animChannel.keyScalings[scaleIdx] = scale;
  }
}

void ModelExporter::ExportAnimation(Animation& geoAnim) {
  flatbuffers::FlatBufferBuilder builder;

  auto name = builder.CreateString(geoAnim.name);

	const auto& transform = geoAnim.globalInverseTransform;
  auto global_inverse_transform = GameResource::Matrix(
      transform[0][0], transform[0][1], transform[0][2], transform[0][3],
      transform[1][0], transform[1][1], transform[1][2], transform[1][3],
      transform[2][0], transform[2][1], transform[2][2], transform[2][3],
      transform[3][0], transform[3][1], transform[3][2], transform[3][3]);

	// Serialize animation channels
  std::vector<flatbuffers::Offset<GameResource::AnimationChannel>>
      channelVector(geoAnim.animationChannels.size());
  for (size_t i = 0; i < geoAnim.animationChannels.size(); ++i)
  {
    auto& channel = geoAnim.animationChannels[i];

		auto nodeName = builder.CreateString(channel.nodeName);

		std::vector<GameResource::KeyPosition> keyPositions(
        channel.numKeyPositions);
    for (int k = 0; k < channel.numKeyPositions; ++k)
    {
      keyPositions[k] = GameResource::KeyPosition(
          channel.keyPositions[k].position, channel.keyPositions[k].timeStamp);
		}
    auto flatKeyPositions = builder.CreateVectorOfStructs(keyPositions);

		std::vector<GameResource::KeyRotation> keyRotations(
        channel.numKeyRotations);
    for (int k = 0; k < channel.numKeyRotations; ++k)
    {
      keyRotations[k] = GameResource::KeyRotation(
          channel.keyRotations[k].orientation, channel.keyRotations[k].timeStamp);
    }
    auto flatKeyRotations = builder.CreateVectorOfStructs(keyRotations);

		std::vector<GameResource::KeyScaling> keyScaling(
        channel.numKeyScalings);
    for (int k = 0; k < channel.numKeyScalings; ++k)
    {
      keyScaling[k] = GameResource::KeyScaling(
          channel.keyScalings[k].scaling, channel.keyScalings[k].timeStamp);
    }
    auto flatKeyScalings = builder.CreateVectorOfStructs(keyScaling);

    channelVector[i] = GameResource::CreateAnimationChannel(
        builder, channel.boneId, nodeName, channel.numKeyPositions,
        channel.numKeyRotations, channel.numKeyScalings, flatKeyPositions,
        flatKeyRotations, flatKeyScalings);
  }
  auto animation_channels = builder.CreateVector(channelVector);

  auto animation = GameResource::CreateAnimation(
      builder, name, geoAnim.duration, geoAnim.ticksPerSecond,
      &global_inverse_transform, animation_channels);

  builder.Finish(animation);

  // Write to file
  std::string exportPath = GetExportPath(geoAnim.path);
  std::ofstream ofs(exportPath, std::ios::binary);
  ofs.write((char*)builder.GetBufferPointer(), builder.GetSize());
  ofs.close();

  // Generate the model mesh info file
  GenerateAnimationInfoFile(geoAnim);
}

void ModelExporter::GenerateAnimationInfoFile(Animation& geoAnim) {
  ordered_json j;

  j["uuid"] = GetUUID(geoAnim.path);
  j["import_path"] = geoAnim.path;
  j["resource_type"] = "animation";

  ordered_json details;
  details["name"] = geoAnim.name;

  details["duration"] = geoAnim.duration;
  details["ticks_per_second"] = geoAnim.ticksPerSecond;
  details["global_inverse_transform"] = {geoAnim.globalInverseTransform[0][0],
                                         geoAnim.globalInverseTransform[0][1],
                                         geoAnim.globalInverseTransform[0][2],
                                         geoAnim.globalInverseTransform[0][3],
                                         geoAnim.globalInverseTransform[1][0],
                                         geoAnim.globalInverseTransform[1][1],
                                         geoAnim.globalInverseTransform[1][2],
                                         geoAnim.globalInverseTransform[1][3],
                                         geoAnim.globalInverseTransform[2][0],
                                         geoAnim.globalInverseTransform[2][1],
                                         geoAnim.globalInverseTransform[2][2],
                                         geoAnim.globalInverseTransform[2][3],
                                         geoAnim.globalInverseTransform[3][0],
                                         geoAnim.globalInverseTransform[3][1],
                                         geoAnim.globalInverseTransform[3][2],
                                         geoAnim.globalInverseTransform[3][3]};

	for (const AnimationChannel& channel : geoAnim.animationChannels)
  {
    ordered_json channel_info;
    channel_info["node_name"] = channel.nodeName;
    channel_info["bone_id"] = channel.boneId;

		channel_info["num_key_positions"] = channel.numKeyPositions;
		channel_info["num_key_rotations"] = channel.numKeyRotations;
		channel_info["num_key_scaling"] = channel.numKeyScalings;

		for (const KeyPosition& pos : channel.keyPositions)
    {
      ordered_json pos_info;
      pos_info["position"] = pos.position;
      pos_info["time_stamp"] = pos.timeStamp;
      channel_info["key_positions"].push_back(pos_info);
		}

		for (const KeyRotation& rot : channel.keyRotations)
    {
      ordered_json rot_info;
      rot_info["orientation"] = rot.orientation;
      rot_info["time_stamp"] = rot.timeStamp;
      channel_info["key_rotations"].push_back(rot_info);
    }

		for (const KeyScaling& scale : channel.keyScalings)
    {
      ordered_json scale_info;
      scale_info["scaling"] = scale.scaling;
      scale_info["time_stamp"] = scale.timeStamp;
      channel_info["key_scalings"].push_back(scale_info);
    }

		details["animation_channels"].push_back(channel_info);
	}

  j["details"] = details;

  std::ofstream o(GetExportPath(geoAnim.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

size_t ModelExporter::GetMaxNodeCount(const aiScene* scene)
{
  size_t maxNodeCount{0};

  std::function<void(const aiNode*)> traverseNode = [&](const aiNode* node) {
    maxNodeCount++;

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
      traverseNode(node->mChildren[i]);
    }
  };

  traverseNode(scene->mRootNode);

  return maxNodeCount;
}
