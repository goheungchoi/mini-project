#include<utility.hlsli>
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
    Matrix shadowView;
    Matrix shadowProjection;
};

cbuffer World : register(b1)
{
    Matrix world;
};

cbuffer PixelData : register(b2)
{
    float alphaCutoff;
    float3 padding1;
    float4 color;
}

cbuffer BoneMatrix : register(b3)
{
    Matrix boneMatrix[127];
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

uint querySpecularTextureLevels()
{
    uint width, height, levels;
    evnSpecularIBLTexture.GetDimensions(0, width, height, levels);
    return levels;
}
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
    float depthColor = deferredAlbedoDepth.Sample(samAnisotropy, input.uv).a;
    float3 worldPos = GetWorldPosition(input.uv, depthColor.r);
    
    float3 albedo = deferredAlbedoDepth.Sample(samAnisotropy, input.uv).xyz;
    albedo = pow(albedo, 2.2);
    if (depthColor.r == 0)
    {
        return float4(pow(albedo, 1 / 2.2), 1.f);
    }
    //gamma correction
    float3 normal = deferredNormal.Sample(samAnisotropy, input.uv).rgb;
    float metallic = deferredMaterial.Sample(samAnisotropy, input.uv).r;
    float roughness = deferredMaterial.Sample(samAnisotropy, input.uv).g;

    float4 emissive = deferredEmissive.Sample(samAnisotropy, input.uv);
    
    //Fresnel reflectance
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 directLighting = 0.f;
    float3 ambientLighting = 0.f;
    
    float3 lightOut = normalize(cameraPosition.xyz - worldPos.xyz);
    float NdotV = max(0.0,dot(normal, lightOut));
    float3 lightReflection = 2.f * NdotV * normal - lightOut;
    float3 lightIn = normalize(-mainDirectionalLight.direction.xyz);
    float3 Lradiance = mainDirectionalLight.color.xyz * mainDirectionalLight.intensity.xyz;
    //pbr
    float3 lightHalf = normalize(lightIn + lightOut);
    float NdotL = max(0.0,dot(normal, lightIn));
    float NDotH = max(0.0,dot(normal, lightHalf));
    float3 F = FresnelFactor(max(0.0, dot(lightHalf, lightOut)), F0);
    float D = NormalDistributionFunction(normal, lightHalf, max(0.01, roughness));
    float G = GAFDirect(normal, lightOut, lightIn, roughness);
    float3 kd = lerp(float3(1.0, 1.0, 1.0) - F, float3(0.0, 0.0, 0.0), metallic);
    float3 diffuseBRDF = kd * albedo / PI;
    float3 specularBRDF = (F * D * G) / max(Epsilon, (4.0 * NdotL * NdotV));
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * NdotL;
    //ibl
    float3 irradiance = evnIrradianceTexture.Sample(samAnisotropy, normal).rgb;
    F = FresnelFactor(NdotV, F0);
        //금속일수록 specular항을 그대로, 표면 산란을 줄인다.
    kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
    float3 IBLdiffuse = kd * albedo * irradiance;
        //mip map 레벨 구하기
    uint specularTextureLevels = querySpecularTextureLevels();
        //레벨에 따라 roughness를 곱해서 반사의 선명함 정도 구하기 -> roughness가0이면 제일 큰 해상도의 큐브맵 반사
    float3 specularIrradiance = evnSpecularIBLTexture.SampleLevel(samAnisotropy, lightReflection, roughness * specularTextureLevels).rgb;
        //look up table pbr은정적이므로 미리 계산된 텍스쳐로 uv좌표의 근사값을 사용
    float2 IBLSpecularBRDF = evnSpecularBRDF.Sample(samClamp, float2(NdotV, roughness)).rg;
    float3 specularIBL = (F0 * IBLSpecularBRDF.x + IBLSpecularBRDF.y) * specularIrradiance;
        //더하기
    
    ambientLighting += (IBLdiffuse + specularIBL)*0.2;
    //ambientLighting += 0;
    
    
    float4 positionShadow = deferredShadowPosition.Sample(samLinear, input.uv);
    float currShadowDepth = positionShadow.z; // / positionShadow.w;
    float2 uv = positionShadow.xy; // / positionShadow.w;
    //y뒤집기
    uv.y = -uv.y;
    //-1~1 ->0~1
    uv = uv * 0.5f + 0.5f;
    //그림자 factor
    float shadowFactor = 1.f;
    //커버할수 있는 영역이 아니면 처리 x
    if (uv.x >= 0.f && uv.x <= 1.f && uv.y >= 0.f && uv.y <= 1.f)
    {
        float2 offset[9] =
        {
            float2(-1, -1), float2(0, -1), float2(1, -1),
                float2(-1, 0), float2(0, 0), float2(1, 0),
                float2(-1, 1), float2(0, 1), float2(1, 1)
        };
        uint width, height, levels;
        texShadow.GetDimensions(width, height);
        float texelSize = 1.0 / width; //텍셀 크기
        shadowFactor = 0.f;
            [unroll]
        for (int i = 0; i < 9; i++)
        {
            float2 sampleUV = uv + offset[i] * texelSize; //오프셋 계산
                //sampleCmpLevelZero로 pcf샘플링
            shadowFactor += texShadow.SampleCmpLevelZero(samComparison, sampleUV, currShadowDepth - 0.001);

        }
        shadowFactor = shadowFactor / 9.f;
            
    }
    float4 finalColor;
    float4 temp = float4(float3(directLighting*shadowFactor + ambientLighting), 1.f) + emissive;
    temp = pow(temp, 1.0 / 2.2);
    finalColor = float4(temp.rgb, 1.f);
    return finalColor;
}
#endif
//---------------------------define Deffered--------------------------------------------
#ifdef Deffered
struct DEFFERED_PS_OUT
{
    float4 AlbedoDepth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Material : SV_Target2;
    float4 Emissive : SV_Target3;
    float4 ShadowPosition : SV_Target4;
};

DEFFERED_PS_OUT ps_main(PS_INPUT input)
{
    DEFFERED_PS_OUT output = (DEFFERED_PS_OUT) 0;
    float depth = input.position.z;
    float4 albedo = texAlbedo.Sample(samLinear, input.uv);
    clip(albedo.a - alphaCutoff);
    output.AlbedoDepth.xyz = albedo;
    output.AlbedoDepth.a = depth;
    output.ShadowPosition = input.positionShadow / input.positionShadow.w;
    float metalic = texMetallicRoughness.Sample(samAnisotropy, input.uv).r;
    float roughness = texMetallicRoughness.Sample(samAnisotropy, input.uv).g;
 
    float3 N = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samAnisotropy, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 T = normalize(input.worldTangent);
        float3 B = normalize(input.worldBitangent);
        float3 normalTexColor;
        normalTexColor = (normalTexture.rgb * 2.0) - 1.0;
        normalTexColor = normalize(normalTexColor);
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
#ifdef Skinning
    uint4 boneIndices0 = uint4(boneIDBuffer[input.vertexID * 8 + 0],
                               boneIDBuffer[input.vertexID * 8 + 1],
                               boneIDBuffer[input.vertexID * 8 + 2],
                               boneIDBuffer[input.vertexID * 8 + 3]);
    uint4 boneIndices1 = uint4(boneIDBuffer[input.vertexID * 8 + 4],
                               boneIDBuffer[input.vertexID * 8 + 5],
                               boneIDBuffer[input.vertexID * 8 + 6],
                               boneIDBuffer[input.vertexID * 8 + 7]);

    float4 boneWeights0 = float4(boneWeightBuffer[input.vertexID * 8 + 0],
                                 boneIDBuffer[input.vertexID * 8 + 1],
                                 boneIDBuffer[input.vertexID * 8 + 2],
                                 boneIDBuffer[input.vertexID * 8 + 3]);
    float4 boneWeights1 = float4(boneIDBuffer[input.vertexID * 8 + 4],
                                 boneIDBuffer[input.vertexID * 8 + 5],
                                 boneIDBuffer[input.vertexID * 8 + 6],
                                 boneIDBuffer[input.vertexID * 8 + 7]);
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        matWolrd += boneWeights0[i] * boneMatrix[boneIndices0[i]];
    }

    // Apply skinning for next 4 bones
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        matWolrd += boneWeights1[i] * boneMatrix[boneIndices1[i]];
    }
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
    output.positionShadow = mul(output.worldPosition,
    shadowView);
    output.positionShadow = mul(output.positionShadow, shadowProjection);
    return output;
}
//---------------------------define Transparency--------------------------------------------
#ifdef Transparency

float4 ps_main(PS_INPUT input) : SV_TARGET0
{
    float3 albedo = texAlbedo.Sample(samLinear, input.uv).xyz;
    albedo = pow(albedo, 2.2);
    if (length(albedo) == 0.f)
    {
        albedo = input.color.rgb;
    }
    //gamma correction
    float metallic = texMetallicRoughness.Sample(samLinear, input.uv).x;
    float roughness = texMetallicRoughness.Sample(samLinear, input.uv).y;
    float3 normal = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samLinear, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 normalTexColor;
        normalTexColor.xyz = normalTexture.rgb * 2.0 - 1.0;
        normalTexColor = normalize(normalTexColor);
        float3 tangent = normalize(input.worldTangent);
        float3 bitangent = normalize(input.worldBitangent);

        float3x3 TBN = float3x3(tangent, bitangent, normal);
        normal = normalize(mul(normalTexColor, TBN));
    }
    //Fresnel reflectance
    float3 f0 = lerp(Fdielectric, albedo, metallic);
    float3 directLighting = 0.f;
    float3 ambientLighting = 0.f;
    //pbr
    float3 lightOut = normalize(cameraPosition.xyz - input.worldPosition.xyz);
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
    //ibl
    float3 irradiance = evnIrradianceTexture.Sample(samAnisotropy, normal).rgb;
    F = FresnelFactor(NdotV, f0);
        //금속일수록 specular항을 그대로, 표면 산란을 줄인다.
    kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
    float3 IBLdiffuse = kd * albedo * irradiance;
        //mip map 레벨 구하기
    uint specularTextureLevels = querySpecularTextureLevels();
        //레벨에 따라 roughness를 곱해서 반사의 선명함 정도 구하기 -> roughness가0이면 제일 큰 해상도의 큐브맵 반사
    float3 specularIrradiance = evnSpecularIBLTexture.SampleLevel(samAnisotropy, lightReflection, roughness * specularTextureLevels).rgb;
        //look up table pbr은정적이므로 미리 계산된 텍스쳐로 uv좌표의 근사값을 사용
    float2 IBLSpecularBRDF = evnSpecularBRDF.Sample(samClamp, float2(NdotV, roughness)).rg;
    float3 specularIBL = (f0 * IBLSpecularBRDF.x + IBLSpecularBRDF.y) * specularIrradiance;
        //더하기
    float4 ambientFactor = texOcclusion.Sample(samLinear, input.uv);
    
    if (length(ambientFactor) == 0.f)
    {
        ambientFactor = float4(1.f, 1.f, 1.f, 1.f);
    }
    
    ambientLighting += (IBLdiffuse + specularIBL);

    float currShadowDepth = input.positionShadow.z / input.positionShadow.w;
    float2 uv = input.positionShadow.xy / input.positionShadow.w;
    //y뒤집기
    uv.y = -uv.y;
    //-1~1 ->0~1
    uv = uv * 0.5f + 0.5f;
    //그림자 factor
    float shadowFactor = 1.f;
    if (uv.x >= 0.f && uv.x <= 1.f && uv.y >= 0.f && uv.y <= 1.f)
    {
        float2 offset[9] =
        {
            float2(-1, -1), float2(0, -1), float2(1, -1),
                float2(-1, 0), float2(0, 0), float2(1, 0),
                float2(-1, 1), float2(0, 1), float2(1, 1)
        };
        uint width, height, levels;
        texShadow.GetDimensions(width, height);
        float texelSize = 1.0 / width; //텍셀 크기
        shadowFactor = 0.f;
            [unroll]
        for (int i = 0; i < 9; i++)
        {
            float2 sampleUV = uv + offset[i] * texelSize; //오프셋 계산
                //sampleCmpLevelZero로 pcf샘플링
            shadowFactor += texShadow.SampleCmpLevelZero(samComparison, sampleUV, currShadowDepth - 0.001);

        }
        shadowFactor = shadowFactor / 9.f;
    }
    float alpha = 1.0;
    float4 alphaColor = texAlbedo.Sample(samLinear, input.uv);
    alpha = alphaColor.a;
    
    
    
    float4 finalColor;
    float4 emissive = texEmissive.Sample(samLinear, input.uv);
    float4 temp = float4(float3((directLighting * shadowFactor) + ambientLighting), 1.f) + emissive;
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
float4 skybox_ps_main(SKYBOX_PS_INPUT input):SV_Target0
{
    return evnTexture.Sample(samAnisotropy, input.texCoord);
}
#endif

//---------------------------define Shadow--------------------------------------------
#ifdef Shadow
PS_INPUT shadow_vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
       
    float4 pos = input.position;
    float4x4 matWolrd = world;
#ifdef Skinning
    uint4 boneIndices0 = uint4(boneIDBuffer[input.vertexID * 8 + 0],
                               boneIDBuffer[input.vertexID * 8 + 1],
                               boneIDBuffer[input.vertexID * 8 + 2],
                               boneIDBuffer[input.vertexID * 8 + 3]);
    uint4 boneIndices1 = uint4(boneIDBuffer[input.vertexID * 8 + 4],
                               boneIDBuffer[input.vertexID * 8 + 5],
                               boneIDBuffer[input.vertexID * 8 + 6],
                               boneIDBuffer[input.vertexID * 8 + 7]);

    float4 boneWeights0 = float4(boneWeightBuffer[input.vertexID * 8 + 0],
                                 boneIDBuffer[input.vertexID * 8 + 1],
                                 boneIDBuffer[input.vertexID * 8 + 2],
                                 boneIDBuffer[input.vertexID * 8 + 3]);
    float4 boneWeights1 = float4(boneIDBuffer[input.vertexID * 8 + 4],
                                 boneIDBuffer[input.vertexID * 8 + 5],
                                 boneIDBuffer[input.vertexID * 8 + 6],
                                 boneIDBuffer[input.vertexID * 8 + 7]);
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        matWolrd += boneWeights0[i] * boneMatrix[boneIndices0[i]];
    }

    // Apply skinning for next 4 bones
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        matWolrd += boneWeights1[i] * boneMatrix[boneIndices1[i]];
    }
#endif    
    pos = mul(pos, matWolrd);
    pos = mul(pos, shadowView);
    pos = mul(pos, shadowProjection);
    output.position = pos;
    return output;
}
#endif
//---------------------------define WireFrame--------------------------------------------
#ifdef WireFrame
float4 wire_frame_ps_main(PS_INPUT input) : SV_Target0
{
    return color;
}
#endif