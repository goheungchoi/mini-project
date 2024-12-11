#pragma once

bool PopulateShaderVariants(
    size_t numDefineOptions, const ShaderDefineOption* defineOptions,
    std::vector<std::vector<ShaderDefine>>* outVariants);
