#include "ModelExport.h"

#include <stb_image.h>

#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
using namespace nlohmann;

#include "UUID.h"

// TODO: Animation data
// TODO: Skeletal mesh
bool ModelExporter::ExportModel(const char* path, ModelFileFormat fileFormat,
                                bool preCalculateVertex,
                                bool extractBones)
{
	// Extract bone flag state change
  _extractBones = extractBones;

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
  _geoModel.nodes.reserve(GetMaxNodeCount(pScene));

	// Extract bones option
  /*if (_extractBones)
  {
    ExtractSkeletalBones(pScene);
  }*/

	// Process the scene
  ProcessScene(pScene);

	

	// Export the geometry model
  ExportGeometryModel(_geoModel);

  return true;
}

void ModelExporter::ProcessScene(const aiScene* scene) {
  // Start from the root node and progress recursively.
  GeometryNode geoNode;
  geoNode.name = scene->mRootNode->mName.C_Str();
  geoNode.level = 0;
  geoNode.parent = -1;
  geoNode.myIndex = 0;
  geoNode.firstChild = -1;
  geoNode.nextSibling = -1;
  memcpy(geoNode.transform, &scene->mRootNode->mTransformation.a1,
         16 * sizeof(float));
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
    geoNode.level = parentGeoNode.level + 1;
    geoNode.parent = parentGeoNode.myIndex;
    geoNode.myIndex = geoModel.nodes.size();
    geoNode.firstChild = -1;
    geoNode.nextSibling = i < (node->mNumChildren - 1) ? geoNode.myIndex + 1 : -1;
    memcpy(geoNode.transform, &node->mTransformation.a1, 16 * sizeof(float));
		geoModel.nodes.push_back(geoNode);
  }

	// Process the child nodes
	for (int i = 0; i < node->mNumChildren; ++i)
  {
    int childIndex = parentGeoNode.firstChild + i;
    ProcessNode(geoModel, geoModel.nodes[childIndex], node->mChildren[i], scene);
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
  geoMesh.aabb = AABB{
      .min = {mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z},
      .max = {mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z}
	};

	// Get vertices and indices
  std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3);

  // Process vertices;
  for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex v{
        .position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                     mesh->mVertices[i].z, 1.0},
        .normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                   mesh->mNormals[i].z}
    };

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

	// Move the vertices and indices data to the mesh
  geoMesh.vertices = std::move(vertices);
  geoMesh.indices = std::move(indices);

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

	// Get the virtual path of the material.
	// It will be _directory/mat_name.mat
  geoMat.path = (_directory / material->GetName().C_Str()).string() + ".mat";

  // Get name
  geoMat.name = material->GetName().C_Str();

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
  int occlusionTextureCount = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
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
  int emissiveTextureCount = material->GetTextureCount(aiTextureType_EMISSION_COLOR);
  if (emissiveTextureCount > 0)
  {
    ProcessMaterialTexture(geoModel, geoMat, material,
                           aiTextureType_EMISSION_COLOR,
                           scene);
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
                                        Material& geoMat, aiMaterial* material,
                                        aiTextureType type,
                                        const aiScene* scene)
{
  aiString path;
  material->GetTexture(type, 0, &path);

  Texture texture
  {
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

void ModelExporter::ExportGeometryModel(GeometryModel& geoModel) {
  flatbuffers::FlatBufferBuilder builder;

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

		std::vector<flatbuffers::Offset<flatbuffers::String>> meshesVector;
    meshesVector.reserve(node.meshPaths.size());
    for (auto& meshPath : node.meshPaths)
    {
      meshesVector.push_back(builder.CreateString(meshPath));
		}

		auto meshes = builder.CreateVector(meshesVector);
	
		auto geometryNode = GameResource::CreateGeometryNode(
        builder, name, node.level, node.parent, node.firstChild,
        node.nextSibling, meshes);
    nodesVector.push_back(geometryNode);
	}

	auto name = builder.CreateString(geoModel.name);
  auto nodes = builder.CreateVector(nodesVector);
	auto meshes = builder.CreateVector(meshPaths);
  auto materials = builder.CreateVector(materialPaths);
  auto textures = builder.CreateVector(texturePaths);

	auto model = GameResource::CreateGeometryModel(builder, name, nodes, meshes,
                                                 materials, textures);

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

	auto aabb_min = GameResource::CreateVec3(
      builder, geoMesh.aabb.min[0], geoMesh.aabb.min[1], geoMesh.aabb.min[2]);
  auto aabb_max = GameResource::CreateVec3(
      builder, geoMesh.aabb.max[0], geoMesh.aabb.max[1], geoMesh.aabb.max[2]);
  auto aabb = GameResource::CreateAABB(builder, aabb_min, aabb_max);

	std::vector<flatbuffers::Offset<GameResource::Vertex>> verticesVector(
      geoMesh.vertices.size());
  for (size_t i = 0; i < geoMesh.vertices.size(); ++i)
  {
    auto& v = geoMesh.vertices[i];

    auto position = GameResource::CreateVec4(
        builder, v.position[0], v.position[1], v.position[2], v.position[3]);
    auto normal = GameResource::CreateVec3(builder, v.normal[0], v.normal[1],
                                           v.normal[2]);
    auto tangent = GameResource::CreateVec3(builder, v.tangent[0], v.tangent[1],
                                            v.tangent[2]);
    auto bitangent = GameResource::CreateVec3(builder, v.bitangent[0],
                                              v.bitangent[1], v.bitangent[2]);
    auto texcoord =
        GameResource::CreateTexCoord(builder, v.texcoord[0], v.texcoord[1]);
    auto color = GameResource::CreateVec4(builder, v.color[0], v.color[1],
                                          v.color[2], v.color[3]);
    auto vertex = GameResource::CreateVertex(builder, position, normal, tangent,
                                             bitangent, texcoord, color);
    verticesVector[i] = vertex;
	}

	auto vertices = builder.CreateVector(verticesVector);
  auto indices = builder.CreateVector(geoMesh.indices);
  auto material = builder.CreateString(geoMesh.materialPath);

	auto mesh =
      GameResource::CreateMesh(builder, name, aabb, vertices, indices, material);

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

	auto albedoFactor = GameResource::CreateVec4(
      builder, geoMat.albedoFactor[0], geoMat.albedoFactor[1],
      geoMat.albedoFactor[2], geoMat.albedoFactor[3]);
  auto albedoTexture = builder.CreateString(geoMat.albedoTexture);

	auto metallicRoughnessTexture =
      builder.CreateString(geoMat.metallicRoughnessTexture);
  auto normalTexture = builder.CreateString(geoMat.normalTexture);
  auto occlusionTexture = builder.CreateString(geoMat.occlusionTexture);
  auto emissiveTexture = builder.CreateString(geoMat.emissiveTexture);
	
	auto material = GameResource::CreateMaterial(
      builder, name, albedoFactor, albedoTexture, geoMat.metallicFactor,
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

std::string ModelExporter::GetExportPath(std::string path)
{
  std::replace(path.begin(), path.end(), '/', '\\');

  std::string strUUID = GenerateUUIDFromName(path).ToString();

  fs::path resourceSubDir = fs::absolute(resourceDir) / strUUID.substr(0, 2);

  if (!fs::exists(resourceSubDir))
  {
    fs::create_directories(resourceSubDir);
  }

  fs::path exportPath = resourceSubDir / strUUID;

	return exportPath.string();
}

void ModelExporter::GenerateGeometryModelInfoFile(GeometryModel& geoModel) {
  ordered_json j;

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

	j["details"] = details;

	std::ofstream o(GetExportPath(geoModel.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::GenerateModelMeshInfoFile(Mesh& geoMesh)
{
  ordered_json j;

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

	j["details"] = details;

	std::ofstream o(GetExportPath(geoMesh.path) + ".info");
  o << std::setw(4) << j << std::endl;
}

void ModelExporter::GenerateModelMaterialInfoFile(Material& geoMat)
{
  ordered_json j;

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

void ModelExporter::ExtractSkeletonBonesAndWeights(GeometryModel& geoModel,
                                           Mesh& geoMesh, aiMesh* mesh,
                                           const aiScene* scene)
{
	// Build a necessity map.
  std::unordered_map<std::string, bool> nodeNecessityMap;
  
  std::function<void(const aiNode*)> buildNodeNecessityMap =
      [&](const aiNode* node) {
        nodeNecessityMap[node->mName.C_Str()] = false;
  
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
          buildNodeNecessityMap(node->mChildren[i]);
        }
      };
  buildNodeNecessityMap(scene->mRootNode);

	// Mark necessary nodes.
  for (unsigned int i = 0; i < mesh->mNumBones; ++i)
  {
    const aiBone* bone = mesh->mBones[i];
    const std::string boneName = bone->mName.C_Str();

		// Fine the node corresponding to the bone.
		const aiNode* boneNode = scene->mRootNode->FindNode(bone->mName);
    while (boneNode)
    {
			// Mark the bone node as `true` in the necessity map
      nodeNecessityMap[boneNode->mName.C_Str()] = true;

			// Mark all of its parents until the mesh's node is found or
			// the parent of the mesh's node is found.
			boneNode = boneNode->mParent;
		}
	
	}


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

//void ModelExporter::ExtractSkeletalBones(const aiScene* scene)
//{
//	
//	std::unordered_map<std::string, bool> necessityMap;
//
//	std::function<void(const aiNode*)> buildNecessityMap =
//      [&](const aiNode* node) {
//        necessityMap[node->mName.C_Str()] = false;
//
//        for (unsigned int i = 0; i < node->mNumChildren; ++i)
//        {
//          buildNecessityMap(node->mChildren[i]);
//        }
//      };
//  buildNecessityMap(scene->mRootNode);
//
//
//
//
//}
//
//void ModelExporter::ProcessSkeletonNode(GeometryModel& geoModel, aiNode* node,
//                                        const aiScene* scene)
//{
//  //// Process the meshes of the current geo node.
//  //for (int i = 0; i < node->mNumMeshes; ++i)
//  //{
//  //  aiMesh* mesh = scene->mMeshes[i];
//  //  ProcessSkeletonMesh(mesh, scene);
//  //}
//
//	// Process the child nodes
//  for (int i = 0; i < node->mNumChildren; ++i)
//  {
//    ProcessSkeletonNode(node->mChildren[i], scene);
//  }
//}
//
//void ModelExporter::ProcessSkeletonMesh(GeometryModel& geoModel, aiMesh* mesh,
//                                        const aiScene* scene)
//{
//
//
//}


