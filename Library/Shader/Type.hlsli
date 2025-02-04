SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);
SamplerState samAnisotropy : register(s2);
SamplerComparisonState samComparison : register(s3);

Texture2D texAlbedo : register(t0);
Texture2D texNormal : register(t1);
Texture2D texMetallicRoughness : register(t2);
Texture2D texEmissive : register(t3);
Texture2D texOcclusion : register(t4);
//skybox
TextureCube evnTexture : register(t5);
Texture2D evnSpecularBRDF : register(t6);
TextureCube evnIrradianceTexture : register(t7);
TextureCube evnSpecularIBLTexture : register(t8);
Texture2D texShadow : register(t9);
//deferred
Texture2D deferredAlbedoDepth : register(t10);
Texture2D deferredNormal : register(t11);
Texture2D deferredMaterial : register(t12);
Texture2D deferredEmissive : register(t13);
Texture2D deferredShadowPosition : register(t14);

//skinning
StructuredBuffer<uint> boneIDBuffer : register(t15);
StructuredBuffer<float> boneWeightBuffer : register(t16);
struct DirectionalLight
{
    float4 direction;
    float4 radiance;
};
struct PointLight
{
    float4 position;
    float4 direction;
    float4 color;
    float intensity;
};

cbuffer Frame : register(b1)
{
    DirectionalLight mainDirectionalLight;
    float4 cameraPosition;
    Matrix view;
    Matrix projection;
    Matrix InverseViewMatrix;
    Matrix InverseProjectionMatrix;
    Matrix shadowView;
    Matrix shadowProjection;
};

cbuffer World : register(b2)
{
    Matrix world;
};

cbuffer PixelData : register(b3)
{
    float alphaCutoff;
    float metalicFactor;
    float roughnessFactor;
    float padding1;
    float4 albedoFactor;
}

cbuffer BoneMatrix : register(b4)
{
    Matrix boneMatrix[512];
}

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
    uint vertexID : SV_VertexID;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 positionShadow : POSITION;
    float4 color : COLOR;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float3 worldBitangent : BINORMAL;
    float2 uv : TEXCOORD;
    float4 worldPosition : WORLD;
};
