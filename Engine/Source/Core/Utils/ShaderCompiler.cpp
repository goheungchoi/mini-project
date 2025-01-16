#include "Core/Utils/ShaderCompiler.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static void Helper(std::vector<std::vector<uint8_t>>& outCombs,
                   const size_t numVars, const std::vector<uint8_t>& selections,
                   size_t currIndex, std::vector<uint8_t>& currComb)
{
  if (currIndex == numVars)
  {
    outCombs.push_back(currComb);
    return;
  }

  size_t nextIndex = currIndex + 1;
  for (uint8_t i = 0; i < selections[currIndex]; ++i)
  {
    currComb.push_back(i);
    Helper(outCombs, numVars, selections, nextIndex, currComb);
    currComb.pop_back();
  }
}

static void GetCombinations(std::vector<std::vector<uint8_t>>& outCombs,
                            const size_t numVars,
                            const std::vector<uint8_t>& selections)
{
  if (numVars == 0 || selections.empty())
    return;

  std::vector<uint8_t> comb;
  comb.reserve(numVars);
  Helper(outCombs, numVars, selections, 0, comb);
}

bool Utils::PopulateShaderVariants(
    size_t numOptions, const ShaderOption* options,
    std::vector<std::vector<ShaderDefine>>& outVariants)
{
  if (numOptions == 0)
    return true;

  // Calculate the number of possible values of each define option,
  // and the total number of the define variants.
  std::vector<uint8_t> optionsNumValues;
  optionsNumValues.resize(numOptions);

  size_t numVariants{1};
  for (size_t i = 0; i < numOptions; ++i)
  {
    switch (options[i].type)
    {
    case ShaderDefineValueType::kNone:
      optionsNumValues[i] = 2;
      numVariants *= 2;
      break;
    case ShaderDefineValueType::kBool:
      optionsNumValues[i] = 2;
      numVariants *= 2;
      break;
    case ShaderDefineValueType::kInteger:
    case ShaderDefineValueType::kFloat:
      optionsNumValues[i] = options[i].values.size();
      numVariants *= options[i].values.size();
      break;
    }
  }

  if (numVariants == 0)
    return false;

  // Calculate the combinations of the options' number of values.
  std::vector<std::vector<uint8_t>> combinations;
  combinations.reserve(numVariants);
  GetCombinations(combinations, numOptions, optionsNumValues);

  // Get all the combinations of the defines
  outVariants.reserve(numVariants);
  for (size_t i = 0; i < numVariants; ++i)
  {
    std::vector<ShaderDefine> variant;
    variant.reserve(numOptions);
    for (size_t j = 0; j < numOptions; ++j)
    {
      switch (options[j].type)
      {
      case ShaderDefineValueType::kNone:
        if (combinations[i][j])
        {
          variant.emplace_back(ShaderDefine{.define = options[j].define,
                                            .type = options[j].type});
        }
        break;
      case ShaderDefineValueType::kBool:
        if (combinations[i][j])
        {
          variant.emplace_back(ShaderDefine{.define = options[j].define,
                                            .type = options[j].type,
                                            .value = true});
        }
        else
        {
          if (combinations[i][j])
          {
            variant.emplace_back(ShaderDefine{.define = options[j].define,
                                              .type = options[j].type,
                                              .value = false});
          }
        }
        break;
      case ShaderDefineValueType::kInteger:
      case ShaderDefineValueType::kFloat:
        variant.emplace_back(
            ShaderDefine{.define = options[j].define,
                         .type = options[j].type,
                         .value = options[j].values[combinations[i][j]]});
        break;
      }
    }
    outVariants.emplace_back(std::move(variant));
  }

  return true;
}

bool Utils::GenerateShaderVariantsFile(const char* shaderDir)
{
  fs::path dir = shaderDir;
  if (!fs::is_directory(dir))
  {
    return false;
  }

  fs::path configPath = dir / "shader_config.json";
  if (!fs::exists(configPath))
  {
    return false;
  }

  std::ifstream f(configPath);
  const json config = json::parse(f);

  // Get the shader options
  const json& options = config["options"];
  size_t numOptions = options.size();

  std::vector<ShaderOption> shaderOptions;

  for (size_t i = 0; i < numOptions; ++i)
  {
    ShaderOption op{};

    op.name = options[i]["name"];
    op.define = options[i]["define"];

    if (auto it = options[i].find("values"); it == options[i].end())
    {
      op.type = ShaderDefineValueType::kNone;
    }
    else if (it.value()[0].is_boolean())
    {
      op.type = ShaderDefineValueType::kBool;
      op.values.push_back(ShaderDefineValue{.vBool = false});
      op.values.push_back(ShaderDefineValue{.vBool = true});
    }
    else if (it.value()[0].is_number_integer())
    {
      op.type = ShaderDefineValueType::kInteger;
      for (size_t j = 0; j < it.value().size(); ++j)
      {
        op.values.push_back(ShaderDefineValue{.vInt = it.value()[j]});
      }
    }
    else
    {
      op.type = ShaderDefineValueType::kFloat;
      for (size_t j = 0; j < it.value().size(); ++j)
      {
        op.values.push_back(ShaderDefineValue{.vFloat = it.value()[j]});
      }
    }

    shaderOptions.emplace_back(std::move(op));
  }

  // Get the shaders
  const json& shaders = config["shaders"];
  size_t numShaders = shaders.size();

  std::vector<ShaderData> shaderVariants;

  for (size_t i = 0; i < numShaders; ++i)
  {
    ShaderData data;

    const json& shader = shaders[i];

    std::string filename = shader["filename"];
    data.name = filename;
    data.path = (dir / filename).string();

    std::string type = shader["type"];
    if (type == "vertex")
    {
      data.type = ShaderType::kVertex;
    }
    else if (type == "geometry")
    {
      data.type = ShaderType::kGeometry;
    }
    else if (type == "hull")
    {
      data.type = ShaderType::kHull;
    }
    else if (type == "tessellator")
    {
      data.type = ShaderType::kTessellator;
    }
    else if (type == "domain")
    {
      data.type = ShaderType::kDomain;
    }
    else if (type == "pixel")
    {
      data.type = ShaderType::kPixel;
    }
    else if (type == "compute")
    {
      data.type = ShaderType::kCompute;
    }
    else
    {
      data.type = ShaderType::kUnknown;
    }

    if (auto it = shader.find("options");
        it != shader.end() && it.value().is_array() && it->size() != 0)
    {
      // Find all the shader options needed
      std::vector<ShaderOption> possibleShaderOptions;

      if (it.value()[0].is_number())
      {
        for (size_t j = 0; j < it->size(); ++j)
        {
          possibleShaderOptions.push_back(shaderOptions[it.value()[j]]);
        }
      }
      else if (it.value()[0].is_string())
      {
        for (size_t j = 0; j < it->size(); ++j)
        {
          auto optionIt =
              std::find_if(shaderOptions.begin(), shaderOptions.end(),
                           [&](const ShaderOption& option) {
                             std::string str = it.value()[j];
                             return option.name == str;
                           });
          if (optionIt != shaderOptions.end())
          {
            possibleShaderOptions.push_back(*optionIt);
          }
        }
      }
      else
      {
        printf("Unknown format for shader options");
        shaderVariants.push_back(data);
        continue;
      }

      // Populate the define variants.
      std::vector<std::vector<ShaderDefine>> defineVariants;
      PopulateShaderVariants(possibleShaderOptions.size(),
                             possibleShaderOptions.data(), defineVariants);

      // Store the define variants.
      for (auto& defines : defineVariants)
      {
        data.defines = std::move(defines);
        shaderVariants.push_back(data);
      }
    }
    else
    {
      shaderVariants.push_back(data);
    }
  }

  // Output the shader variants file.
  json out;

  for (size_t i = 0; i < shaderVariants.size(); ++i)
  {
    out["variants"][i]["name"] = shaderVariants[i].name;
    out["variants"][i]["path"] = shaderVariants[i].path;
    out["variants"][i]["type"] = shaderVariants[i].type;

    for (size_t j = 0; j < shaderVariants[i].defines.size(); ++j)
    {
      const ShaderDefine& define = shaderVariants[i].defines[j];

      out["variants"][i]["defines"][j]["define"] = define.define;
      if (define.type == ShaderDefineValueType::kNone)
      {
        out["variants"][i]["defines"][j]["value"] = nullptr;
      }
      else if (define.type == ShaderDefineValueType::kBool)
      {
        out["variants"][i]["defines"][j]["value"] = define.value.vBool;
      }
      else if (define.type == ShaderDefineValueType::kInteger)
      {
        out["variants"][i]["defines"][j]["value"] = define.value.vInt;
      }
      else if (define.type == ShaderDefineValueType::kFloat)
      {
        out["variants"][i]["defines"][j]["value"] = define.value.vFloat;
      }
    }
  }

  // write prettified JSON to another file
  std::ofstream o(dir / "shader_variants.json");
  o << std::setw(4) << out << std::endl;

  return true;
}
