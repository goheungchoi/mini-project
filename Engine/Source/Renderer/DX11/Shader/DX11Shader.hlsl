#include<utility.hlsli>
SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);
SamplerState samAnisotropy : register(s2);
SamplerComparisonState samComparison : register(s3);

Texture2D texAlbedo : register(t0);
Texture2D texNormal : register(t1);
Texture2D texMetallicRoughness : register(t2);
Texture2D texEmissive : register(t3);
Texture2D texOpacity : register(t3);

struct DirectionalLight
{
    float4 direction;
    float4 color;
    float4 intensity;
};
struct PointLight
{
    float4 position;
    float4 direction;
    float4 color;
    float intensity;
};

cbuffer Frame : register(b0)
{
    DirectionalLight mainDirectionalLight;
    float4 cameraPosition;
    Matrix view;
    Matrix projection;
    Matrix InverseViewMatrix;
    Matrix InverseProjectionMatrix;
};

cbuffer World : register(b1)
{
    Matrix world;
#ifdef SKINNING
    Matrix matrixPalleteArray[127]; 
#endif
};

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
#ifdef SKINNING
    uint4 blendIndicies : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
#endif
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float3 worldBitangent : BINORMAL;
    float2 uv : TEXCOORD;
    float4 worldPosition : WORLD;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4x4 matWolrd = world;
#ifdef SKINNING
    matWolrd = mul(input.blendWeights.x, matrixPalleteArray[input.blendIndicies.x]);
    matWolrd += mul(input.blendWeights.y, matrixPalleteArray[input.blendIndicies.y]);
    matWolrd += mul(input.blendWeights.z, matrixPalleteArray[input.blendIndicies.z]);
    matWolrd += mul(input.blendWeights.w, matrixPalleteArray[input.blendIndicies.w]);
#endif
    
    output.position = mul(input.position, matWolrd);
    output.worldPosition = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.worldNormal = normalize(mul(normalize(input.normal), (float3x3) matWolrd));
    output.worldTangent = normalize(mul(normalize(input.tangent), (float3x3) matWolrd));
    output.worldBitangent = normalize(mul(normalize(input.biTangent), (float3x3) matWolrd));
    output.color = input.color;
    output.uv = input.uv;
    return output;
}

float4 ps_main(PS_INPUT input) :SV_TARGET
{
//#ifdef PBR
    float3 albedo = texAlbedo.Sample(samAnisotropy, input.uv).xyz;
    if (length(albedo) == 0.f)
    {
        albedo = input.color.rgb;
    }
    //gamma correction
    albedo = pow(albedo, 2.2);
    float metallic = texMetallicRoughness.Sample(samAnisotropy, input.uv).x;
    float roughness = texMetallicRoughness.Sample(samAnisotropy, input.uv).y;
    
    float3 normal = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samAnisotropy, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 normalTexColor;
        normalTexColor.xyz = normalTexture.rgb * 2.0 - 1.0;

        float3 tangent = normalize(input.worldTangent);
        float3 bitangent = normalize(input.worldBitangent);

        float3x3 TBN = float3x3(tangent, bitangent, normal);
        normal = normalize(mul(normalTexColor, TBN));
    }
    //Fresnel reflectance
    float3 f0 = lerp(Fdielectric, albedo, metallic);
    float3 directLighting = 0.f;
    float3 ambientLighting = 0.f;
    
    float3 lightOut = normalize(cameraPosition.xyz - input.worldPosition.xyz);
    float NdotV = saturate(dot(normal, lightOut));
    float3 lightReflection = 2.f * NdotV * normal - lightOut;
    float3 lightIn = normalize(-mainDirectionalLight.direction.xyz);
    float3 Lradiance = mainDirectionalLight.color.xyz * mainDirectionalLight.intensity.xyz;
    
    float3 lightHalf = normalize(lightIn + lightOut);
    float NdotL = saturate(dot(normal, lightIn));
    float NDotH = saturate(dot(normal, lightHalf));
    float3 F = FresnelFactor(max(0.0, dot(lightHalf, lightOut)), f0);
    float D = NormalDistributionFunction(normal, lightHalf,max(0.01, roughness));
    float G = GAFDirect(normal, lightOut, lightIn, roughness);
    
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo / PI;
    float3 specularBRDF = (F * D * G) / max(Epsilon, (4 * NdotL * NdotV));
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * NdotL;
    float alpha=1.0;
    #ifdef Transparency
    float4 alphaColor = texAlbedo.Sample(samAnisotropy, input.uv);
    alpha = alphaColor.a;
    #endif 
    float4 finalColor;
    float4 emissive = texEmissive.Sample(samAnisotropy, input.uv);
    float4 temp = float4(float3(directLighting + ambientLighting), 1.f) + emissive;
    temp = pow(temp, 1.0 / 2.2);
    finalColor = float4(temp.rgb, alpha);
    if (alpha < 0.1)
    {
        discard;
    }
    return finalColor;
    //#endif
}