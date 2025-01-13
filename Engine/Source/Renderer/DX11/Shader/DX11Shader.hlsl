#include<utility.hlsli>
SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);
SamplerState samAnisotropy : register(s2);
SamplerComparisonState samComparison : register(s3);

Texture2D texAlbedo : register(t0);
Texture2D texNormal : register(t1);
Texture2D texMetallicRoughness : register(t2);
Texture2D texEmissive : register(t3);
Texture2D texOpacity : register(t4);
//skybox
TextureCube evnTexture : register(t5);
Texture2D evnSpecularBRDF : register(t6);
TextureCube evnIrradianceTexture : register(t7);
TextureCube evnSpecularIBLTexture : register(t8);
//deferred
Texture2D deferredDepth : register(t10);
Texture2D deferredAlbedo : register(t11);
Texture2D deferredNormal : register(t12);
Texture2D deferredMaterial : register(t13);
Texture2D deferredEmissive : register(t14);
Texture2D deferredShadowPosition : register(t15);

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
//---------------------------define Quad--------------------------------------------
#ifdef Quad
float3 GetWorldPosition(float2 screenUV, float depth)
{
    // 1. NDC 좌표 복원
    float3 ndcPosition = float3(screenUV * 2.0f - 1.0f, depth);
    ndcPosition.y = -ndcPosition.y;
    // 2. Homogeneous Clip Space 복원
    float4 clipPosition = float4(ndcPosition, 1.0f);

    // 3. 월드 좌표 복원 (View-Projection의 역행렬 사용)
    float4 worldPosition = mul(clipPosition, InverseProjectionMatrix);
    worldPosition = mul(worldPosition, InverseViewMatrix);
    worldPosition /= worldPosition.w; // Homogeneous divide

    return worldPosition.xyz;
}
struct QUAD_VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct QUAD_PS_INPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float4 positionShadow : POSITION;
};

QUAD_PS_INPUT quad_vs_main(QUAD_VS_INPUT input)
{
    QUAD_PS_INPUT output = (QUAD_PS_INPUT) 0;
    input.position.w = 1.0f;
    output.position = input.position;
    output.uv = input.uv;
    return output;
}
float4 quad_ps_main(QUAD_PS_INPUT input) : SV_TARGET0
{
    float4 depthColor = deferredDepth.Sample(samLinear, input.uv);
    float3 worldPos = GetWorldPosition(input.uv, depthColor.r);
    
    float3 albedo = deferredAlbedo.Sample(samLinear, input.uv).xyz;
    if (depthColor.r == 0)
    {
        return float4(pow(albedo, 1 / 2.2), 1.f);
    }
    //gamma correction
    albedo = pow(albedo, 2.2);
    float3 normal = deferredNormal.Sample(samLinear, input.uv).rgb;
    float metallic = deferredMaterial.Sample(samLinear, input.uv).r;
    float roughness = deferredMaterial.Sample(samLinear, input.uv).g;
    float4 emissive = deferredEmissive.Sample(samLinear, input.uv);
    
    //Fresnel reflectance
    float3 f0 = lerp(Fdielectric, albedo, metallic);
    float3 directLighting = 0.f;
    float3 ambientLighting = 0.f;
    
    float3 lightOut = normalize(cameraPosition.xyz - worldPos.xyz);
    float NdotV = saturate(dot(normal, lightOut));
    float3 lightReflection = 2.f * NdotV * normal - lightOut;
    float3 lightIn = normalize(-mainDirectionalLight.direction.xyz);
    float3 Lradiance = mainDirectionalLight.color.xyz * mainDirectionalLight.intensity.xyz;
    
    float3 lightHalf = normalize(lightIn + lightOut);
    float NdotL = saturate(dot(normal, lightIn));
    float NDotH = saturate(dot(normal, lightHalf));
    float3 F = FresnelFactor(max(0.0, dot(lightHalf, lightOut)), f0);
    float D = NormalDistributionFunction(normal, lightHalf, max(0.01, roughness));
    float G = GAFDirect(normal, lightOut, lightIn, roughness);
    
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo / PI;
    float3 specularBRDF = (F * D * G) / max(Epsilon, (4 * NdotL * NdotV));
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * NdotL;
    float4 finalColor;
    float4 temp = float4(float3(directLighting + ambientLighting), 1.f) + emissive;
    temp = pow(temp, 1.0 / 2.2);
    finalColor = float4(temp.rgb, 1.f);
    return finalColor;
}
#endif
//---------------------------define Deffered--------------------------------------------
#ifdef Deffered
struct DEFFERED_PS_OUT
{
    float4 depth : SV_Target0;
    float4 Albedo : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Material : SV_Target3;
    float4 Emissive : SV_Target4;
    float4 ShadowPosition : SV_Target5;
};

DEFFERED_PS_OUT ps_main(PS_INPUT input)
{
    DEFFERED_PS_OUT output = (DEFFERED_PS_OUT) 0;
    float depth = input.position.z;
    output.depth = float4(depth, depth, depth, 1.f);
    output.Albedo = texAlbedo.Sample(samLinear, input.uv);
    //shadow 할때 ㄱㄱ
    //output.ShadowPosition = 
    float metalic = texMetallicRoughness.Sample(samAnisotropy, input.uv).r;
    float roughness = texMetallicRoughness.Sample(samAnisotropy, input.uv).g;
 
    float3 N = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samLinear, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 normalTexColor;
        normalTexColor.xyz = normalTexture.rgb * 2.0 - 1.0;
        float3 B = normalize(input.worldBitangent);
        float3 T = normalize(input.worldTangent);
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normalTexColor, TBN));
    }
    output.Normal = float4(N, 1.f);
    output.Emissive = texEmissive.Sample(samAnisotropy, input.uv);
    output.Material = float4(metalic, roughness, 0.f, 1.f);
    return output;
    
}
#endif

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
//---------------------------define Transparency--------------------------------------------
#ifdef Transparency
float4 ps_main(PS_INPUT input) :SV_TARGET0
{
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
    float4 alphaColor = texAlbedo.Sample(samAnisotropy, input.uv);
    alpha = alphaColor.a;
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
}
#endif 

//---------------------------define SkyBox--------------------------------------------
#ifdef SkyBox
struct SKYBOX_PS_INPUT
{
    float3 texCoord : TEXCOORD;
    float4 positionProj : SV_POSITION;

};

SKYBOX_PS_INPUT skybox_vs_main(VS_INPUT input)
{
    SKYBOX_PS_INPUT output = (SKYBOX_PS_INPUT) 0;
    float4 pos = mul(input.position, world);
    output.texCoord = pos.xyz;
    float3 pos3 = mul(pos.xyz, (float3x3) view);
    pos = mul(float4(pos3, 1.f), projection);
    output.positionProj = pos;
    return output;
}
float4 skybox_ps_main(SKYBOX_PS_INPUT input):SV_Target1
{
    return evnTexture.Sample(samAnisotropy, input.texCoord);
}
#endif