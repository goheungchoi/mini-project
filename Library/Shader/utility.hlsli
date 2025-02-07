#include <Type.hlsli>
//비금속의 프레넬 반사율 기본값
static const float3 Fdielectric = 0.04;
static const float PI = 3.141592654f;
static const float Epsilon = 0.00001;
//to ssao
//static const float3 SSAOKernel[16] =
//{
//    float3(0.5381, 0.1856, -0.4319), float3(0.1379, 0.2486, 0.4430),
//        float3(0.3371, 0.5679, -0.0057), float3(-0.6999, -0.0451, -0.0019),
//        float3(0.0689, -0.1598, -0.8547), float3(0.0560, 0.0069, -0.1843),
//        float3(-0.0146, 0.1402, 0.0762), float3(0.0100, -0.1924, -0.0344),
//        float3(-0.3577, -0.5301, -0.4358), float3(-0.3169, 0.1063, 0.0158),
//        float3(0.0103, -0.5869, 0.0046), float3(-0.0897, -0.4940, 0.3287),
//        float3(0.7119, -0.0154, -0.0918), float3(-0.0533, 0.0596, -0.5411),
//        float3(0.0352, -0.0631, 0.5460), float3(-0.4776, 0.2847, -0.0271)

//};
static const float3 SSAOKernel[16] =
{
    float3(0.587, 0.245, 0.423), float3(-0.348, 0.378, 0.671),
    float3(0.234, -0.156, 0.854), float3(-0.479, 0.632, -0.124),
    float3(0.137, 0.892, -0.307), float3(-0.642, 0.187, 0.374),
    float3(0.214, -0.368, -0.784), float3(0.543, -0.732, 0.120),
    float3(-0.287, -0.476, 0.673), float3(0.726, -0.238, -0.304),
    float3(-0.123, 0.473, -0.789), float3(0.349, -0.832, 0.234),
    float3(-0.586, 0.125, 0.598), float3(0.142, 0.456, 0.759),
    float3(-0.437, -0.523, -0.312), float3(0.672, 0.374, 0.192)
};


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
uint querySpecularTextureLevels()
{
    uint width, height, levels;
    evnSpecularIBLTexture.GetDimensions(0, width, height, levels);
    return levels;
}
float3 GenerateRandomVector(float2 texCoord)
{
    // Use a hash function to get a randoSm float based on texCoord
    float random = frac(sin(dot(texCoord, float2(12.9898, 78.233))) * 43758.5453);
    
    // Convert random value to spherical coordinates
    float phi = random * 2.0f * 3.14159265359f; // Azimuthal angle
    random = frac(sin(dot(texCoord, float2(39.3468, 11.1353))) * 43758.5453);
    float theta = acos(2.0f * random - 1.0f); // Polar angle
    
    // Spherical to Cartesian conversion
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);
    
    return float3(x, y, z);
}

float3 RotateSampleDirection(float3 sampleDir, float2 uv)
{
    // Use UV coordinates to generate a rotation angle based on pixel position
    float angle = uv.x * 3.14159f * 2.0f; // Rotation based on x coordinate

    float s = sin(angle);
    float c = cos(angle);

    // Rotate the sample direction using a 2D rotation matrix
    float3 rotatedDir = float3(
        c * sampleDir.x - s * sampleDir.y,
        s * sampleDir.x + c * sampleDir.y,
        sampleDir.z
    );

    return rotatedDir;
}
