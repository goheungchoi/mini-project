//비금속의 프레넬 반사율 기본값
static const float3 Fdielectric = 0.04;
static const float PI = 3.141592654f;
static const float Epsilon = 0.00001;
float3 FresnelFactor(float cosTheta, float3 F0)
{
    return F0 + (1 - F0) * pow(1 - cosTheta, 5);
}

float NormalDistributionFunction(float3 N, float3 Lh, float roughness)
{
    float alpha = pow(roughness, 2);
    float alphaSq = alpha * alpha;
    float denominator = PI * pow(pow(dot(N, Lh), 2) * (alphaSq - 1) + 1, 2);
    return alphaSq / denominator;
}
float GSchlickGGX(float3 Vector, float3 N, float roughness)
{
    float NDotVec = saturate(dot(N, Vector));
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NDotVec / (NDotVec * (1.0 - k) + k);
}
float GAFDirect(float3 N, float3 Lo, float3 Li, float roughness)
{
    return GSchlickGGX(Lo, N, roughness) * GSchlickGGX(Li, N, roughness);
}