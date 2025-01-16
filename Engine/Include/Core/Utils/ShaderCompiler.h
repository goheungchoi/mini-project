#pragma once

#include "Core/Types/ShaderData.h"

namespace Utils
{

bool PopulateShaderVariants(
    size_t numOptions, const ShaderOption* defineOptions,
    std::vector<std::vector<ShaderDefine>>& outVariants);

bool GenerateShaderVariantsFile(const char* shaderDir);

} // namespace Utils
