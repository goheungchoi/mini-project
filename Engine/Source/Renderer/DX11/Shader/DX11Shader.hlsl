
SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);
SamplerState samAnisotropy : register(s2);
SamplerComparisonState samComparison : register(s3);

Texture2D texAlbedo : register(t0);
Texture2D texNormal : register(t1);
Texture2D texMetallicRoughness : register(t2);
Texture2D texEmissive : register(t3);

cbuffer Frame : register(b0)
{
    float4 cameraPosition;
    Matrix view;
    Matrix projection;
};

cbuffer World : register(b1)
{
    Matrix world;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal: NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    float2 uv : TEXCOORD0;
    float3 color : COLOR;
#ifdef SKINNING
    int4 blendIndicies : BLENDINDICES;
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
    float2 uv : TEXCOORD0;
    float4 worldPosition : WOLRD;
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
    #else
    matWolrd = world;
    #endif
    
    output.position = mul(input.position, matWolrd);
    output.worldPosition = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.worldNormal = normalize(mul(input.normal, (float3x3) matWolrd));
    output.worldTangent = normalize(mul(input.tangent, (float3x3) matWolrd));
    output.worldBitangent = normalize(mul(input.biTangent, (float3x3) matWolrd));
    output.uv = input.uv;
    return output;
}

float4 ps_main(PS_INPUT input)
{
    return float4(1, 0, 0, 1);
}