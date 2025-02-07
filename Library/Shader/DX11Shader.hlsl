#include<utility.hlsli>

//---------------------------define Quad--------------------------------------------
#ifdef Quad
float3 GetWorldPosition(float2 screenUV, float depth)
{
    // 1. NDC ÁÂÇ¥ º¹¿ø
    float3 ndcPosition = float3(screenUV * 2.0f - 1.0f, depth);
    ndcPosition.y = -ndcPosition.y;
    // 2. Homogeneous Clip Space º¹¿ø
    float4 clipPosition = float4(ndcPosition, 1.0f);

    // 3. ¿ùµå ÁÂÇ¥ º¹¿ø (View-ProjectionÀÇ ¿ªÇà·Ä »ç¿ë)
    float4 worldPosition = mul(clipPosition, InverseProjectionMatrix);
    worldPosition = mul(worldPosition, InverseViewMatrix);
    worldPosition /= worldPosition.w; // Homogeneous divide

    return worldPosition.xyz;
}

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
    if (depthColor == 1)
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
    float3 Lradiance = mainDirectionalLight.radiance.xyz * mainDirectionalLight.radiance.w;
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
        //±Ý¼ÓÀÏ¼ö·Ï specularÇ×À» ±×´ë·Î, Ç¥¸é »ê¶õÀ» ÁÙÀÎ´Ù.
    kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
    float3 IBLdiffuse = kd * albedo * irradiance;
        //mip map ·¹º§ ±¸ÇÏ±â
    uint specularTextureLevels = querySpecularTextureLevels();
        //·¹º§¿¡ µû¶ó roughness¸¦ °öÇØ¼­ ¹Ý»çÀÇ ¼±¸íÇÔ Á¤µµ ±¸ÇÏ±â -> roughness°¡0ÀÌ¸é Á¦ÀÏ Å« ÇØ»óµµÀÇ Å¥ºê¸Ê ¹Ý»ç
    float3 specularIrradiance = evnSpecularIBLTexture.SampleLevel(samAnisotropy, lightReflection, roughness * specularTextureLevels).rgb;
        //look up table pbrÀºÁ¤ÀûÀÌ¹Ç·Î ¹Ì¸® °è»êµÈ ÅØ½ºÃÄ·Î uvÁÂÇ¥ÀÇ ±Ù»ç°ªÀ» »ç¿ë
    float2 IBLSpecularBRDF = evnSpecularBRDF.Sample(samClamp, float2(NdotV, roughness)).rg;
    float3 specularIBL = (F0 * IBLSpecularBRDF.x + IBLSpecularBRDF.y) * specularIrradiance;
        //´õÇÏ±â
    
    float4 ambientFactor = texOcclusion.Sample(samLinear, input.uv);
    
    if (length(ambientFactor) == 0.f)
    {
        ambientFactor = float4(1.f, 1.f, 1.f, 1.f);
    }
    
    float ssaoFactor = ssaoMap.Sample(samLinear, input.uv).r;
    if(isSSAO)
        ambientLighting += (IBLdiffuse + specularIBL)*ambientIntencity*ambientFactor.xyz*ssaoFactor;
    else
        ambientLighting += (IBLdiffuse + specularIBL)*ambientIntencity*ambientFactor.xyz;
    //ambientLighting += 0;
    float4 positionShadow = deferredShadowPosition.Sample(samLinear, input.uv);
    float currShadowDepth = positionShadow.z; // / positionShadow.w;
    float2 uv = positionShadow.xy; // / positionShadow.w;
    //yµÚÁý±â
    uv.y = -uv.y;
    //-1~1 ->0~1
    uv = uv * 0.5f + 0.5f;
    //±×¸²ÀÚ factor
    float shadowFactor = 1.f;
    //Ä¿¹öÇÒ¼ö ÀÖ´Â ¿µ¿ªÀÌ ¾Æ´Ï¸é Ã³¸® x
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
        float texelSize = 1.0 / width; //ÅØ¼¿ Å©±â
        shadowFactor = 0.f;
            [unroll]
        for (int i = 0; i < 9; i++)
        {
            float2 sampleUV = uv + offset[i] * texelSize; //¿ÀÇÁ¼Â °è»ê
                //sampleCmpLevelZero·Î pcf»ùÇÃ¸µ
            shadowFactor += texShadow.SampleCmpLevelZero(samComparison, sampleUV, currShadowDepth - 0.001);

        }
        shadowFactor = shadowFactor / 9.f;
            
    }
    float4 finalColor;
    float4 temp = float4(float3(directLighting*shadowFactor + ambientLighting), 1.f) + emissive*emissiveFactor;
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
    if(length(albedo)==0)
    {
        albedo = albedoFactor;
    }
    output.AlbedoDepth.xyz = albedo;
    output.AlbedoDepth.a = depth;
    output.ShadowPosition = input.positionShadow / input.positionShadow.w;
    float4 metallRoughColor = texMetallicRoughness.Sample(samAnisotropy, input.uv);
    float metalic = metallRoughColor.r;
    float roughness = metallRoughColor.g;
    if (length(metallRoughColor) == 0)
    {
        metalic = metalicFactor;
        roughness = roughnessFactor;
    }
    float3 N = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samAnisotropy, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 T = normalize(input.worldTangent);
        float3 B = normalize(input.worldBitangent);
        float3 normalTexColor;
        normalTexColor = (normalize(normalTexture.rgb)* 2.0) - 1.0;
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
    float4x4 matWolrd;
#ifdef Skinning
   matrix boneTransform =  matrix(
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f
    );
    
    uint4 boneIndices0 = uint4(boneIDBuffer[input.vertexID * 8 + 0],
                               boneIDBuffer[input.vertexID * 8 + 1],
                               boneIDBuffer[input.vertexID * 8 + 2],
                               boneIDBuffer[input.vertexID * 8 + 3]);
    //uint4 boneIndices1 = uint4(boneIDBuffer[input.vertexID * 8 + 4],
    //                           boneIDBuffer[input.vertexID * 8 + 5],
    //                           boneIDBuffer[input.vertexID * 8 + 6],
    //                           boneIDBuffer[input.vertexID * 8 + 7]);

    float4 boneWeights0 = float4(boneWeightBuffer[input.vertexID * 8 + 0],
                                 boneWeightBuffer[input.vertexID * 8 + 1],
                                 boneWeightBuffer[input.vertexID * 8 + 2],
                                 boneWeightBuffer[input.vertexID * 8 + 3]);
    //float4 boneWeights1 = float4(boneWeightBuffer[input.vertexID * 8 + 4],
    //                             boneWeightBuffer[input.vertexID * 8 + 5],
    //                             boneWeightBuffer[input.vertexID * 8 + 6],
    //                             boneWeightBuffer[input.vertexID * 8 + 7]);
  
    boneTransform += mul(boneWeights0.x, boneMatrix[boneIndices0.x]);
    boneTransform += mul(boneWeights0.y, boneMatrix[boneIndices0.y]);
    boneTransform += mul(boneWeights0.z, boneMatrix[boneIndices0.z]);
    boneTransform += mul(boneWeights0.w, boneMatrix[boneIndices0.w]);
  

    //boneTransform += mul(boneWeights1.x, boneMatrix[boneIndices1.x]);
    //boneTransform += mul(boneWeights1.y, boneMatrix[boneIndices1.y]);
    //boneTransform += mul(boneWeights1.z, boneMatrix[boneIndices1.z]);
    //boneTransform += mul(boneWeights1.w, boneMatrix[boneIndices1.w]);
  

    matWolrd =  boneTransform;
    float4 totlaPos = mul(input.position, boneTransform);
    matrix wvp = mul(world, view);
    wvp = mul(wvp, projection);
    output.position = mul(totlaPos, wvp);
    output.worldPosition = mul(totlaPos, world);
#else
    matWolrd = world;
    output.position = mul(input.position, matWolrd);
    output.worldPosition = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
#endif
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
    
    if(length(albedo)==0)
    {
        albedo = albedoFactor;
    }
    albedo = pow(albedo, 2.2);
    //gamma correction
    float4 metallRoughColor = texMetallicRoughness.Sample(samAnisotropy, input.uv);
    float metallic = metallRoughColor.r;
    float roughness = metallRoughColor.g;
    if (length(metallRoughColor) == 0)
    {
        metallic = metalicFactor;
        roughness = roughnessFactor;
    }
    float3 normal = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samLinear, input.uv);
    if (length(normalTexture) > 0.f)
    {
        float3 normalTexColor;
        normalTexColor.xyz = normalize(normalTexture.rgb) * 2.0 - 1.0;
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
    float3 Lradiance = mainDirectionalLight.radiance.xyz * mainDirectionalLight.radiance.w;
    
    float3 lightHalf = normalize(lightIn + lightOut);
    float NdotL = saturate(dot(normal, lightIn));
    float NDotH = saturate(dot(normal, lightHalf));
    float3 F = FresnelFactor(max(0.0,  dot(lightHalf, lightOut)), f0);
    float D = NormalDistributionFunction(normal, lightHalf, max(0.01, roughness));
    float G = GAFDirect(normal, lightOut, lightIn, roughness);
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo / PI;
    float3 specularBRDF = (F * D * G) / max(Epsilon, (4 * NdotL * NdotV));
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * NdotL;
    //ibl
    float3 irradiance = evnIrradianceTexture.Sample(samAnisotropy, normal).rgb;
    F = FresnelFactor(NdotV, f0);
        //±Ý¼ÓÀÏ¼ö·Ï specularÇ×À» ±×´ë·Î, Ç¥¸é »ê¶õÀ» ÁÙÀÎ´Ù.
    kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
    float3 IBLdiffuse = kd * albedo * irradiance;
        //mip map ·¹º§ ±¸ÇÏ±â
    uint specularTextureLevels = querySpecularTextureLevels();
        //·¹º§¿¡ µû¶ó roughness¸¦ °öÇØ¼­ ¹Ý»çÀÇ ¼±¸íÇÔ Á¤µµ ±¸ÇÏ±â -> roughness°¡0ÀÌ¸é Á¦ÀÏ Å« ÇØ»óµµÀÇ Å¥ºê¸Ê ¹Ý»ç
    float3 specularIrradiance = evnSpecularIBLTexture.SampleLevel(samAnisotropy, lightReflection, roughness * specularTextureLevels).rgb;
        //look up table pbrÀºÁ¤ÀûÀÌ¹Ç·Î ¹Ì¸® °è»êµÈ ÅØ½ºÃÄ·Î uvÁÂÇ¥ÀÇ ±Ù»ç°ªÀ» »ç¿ë
    float2 IBLSpecularBRDF = evnSpecularBRDF.Sample(samClamp, float2(NdotV, roughness)).rg;
    float3 specularIBL = (f0 * IBLSpecularBRDF.x + IBLSpecularBRDF.y) * specularIrradiance;
        //´õÇÏ±â
    float4 ambientFactor = texOcclusion.Sample(samLinear, input.uv);
    
    if (length(ambientFactor) == 0.f)
    {
        ambientFactor = float4(1.f, 1.f, 1.f, 1.f);
    }
    
    ambientLighting += (IBLdiffuse + specularIBL)*ambientIntencity*ambientFactor;

    float currShadowDepth = input.positionShadow.z / input.positionShadow.w;
    float2 uv = input.positionShadow.xy / input.positionShadow.w;
    //yµÚÁý±â
    uv.y = -uv.y;
    //-1~1 ->0~1
    uv = uv * 0.5f + 0.5f;
    //±×¸²ÀÚ factor
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
        float texelSize = 1.0 / width; //ÅØ¼¿ Å©±â
        shadowFactor = 0.f;
            [unroll]
        for (int i = 0; i < 9; i++)
        {
            float2 sampleUV = uv + offset[i] * texelSize; //¿ÀÇÁ¼Â °è»ê
                //sampleCmpLevelZero·Î pcf»ùÇÃ¸µ
            shadowFactor += texShadow.SampleCmpLevelZero(samComparison, sampleUV, currShadowDepth - 0.001);

        }
        shadowFactor = shadowFactor / 9.f;
    }
    float alpha = 1.0;
    float4 alphaColor = texAlbedo.Sample(samLinear, input.uv);
    if (length(alphaColor)==0)
    {
        alpha = albedoFactor.a;
    }
    else
    {
        alpha = alphaColor.a;
    }
    
    
    
    float4 finalColor;
    float4 emissive = texEmissive.Sample(samLinear, input.uv);
    float4 temp = float4(float3((directLighting * shadowFactor) + ambientLighting), 1.f) + emissive*emissiveFactor;
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
#ifdef Skinning
   matrix boneTransform =  matrix(
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f
    );
    
    uint4 boneIndices0 = uint4(boneIDBuffer[input.vertexID * 8 + 0],
                               boneIDBuffer[input.vertexID * 8 + 1],
                               boneIDBuffer[input.vertexID * 8 + 2],
                               boneIDBuffer[input.vertexID * 8 + 3]);
    //uint4 boneIndices1 = uint4(boneIDBuffer[input.vertexID * 8 + 4],
    //                           boneIDBuffer[input.vertexID * 8 + 5],
    //                           boneIDBuffer[input.vertexID * 8 + 6],
    //                           boneIDBuffer[input.vertexID * 8 + 7]);

    float4 boneWeights0 = float4(boneWeightBuffer[input.vertexID * 8 + 0],
                                 boneWeightBuffer[input.vertexID * 8 + 1],
                                 boneWeightBuffer[input.vertexID * 8 + 2],
                                 boneWeightBuffer[input.vertexID * 8 + 3]);
    //float4 boneWeights1 = float4(boneWeightBuffer[input.vertexID * 8 + 4],
    //                             boneWeightBuffer[input.vertexID * 8 + 5],
    //                             boneWeightBuffer[input.vertexID * 8 + 6],
    //                             boneWeightBuffer[input.vertexID * 8 + 7]);
  
    boneTransform += mul(boneWeights0.x, boneMatrix[boneIndices0.x]);
    boneTransform += mul(boneWeights0.y, boneMatrix[boneIndices0.y]);
    boneTransform += mul(boneWeights0.z, boneMatrix[boneIndices0.z]);
    boneTransform += mul(boneWeights0.w, boneMatrix[boneIndices0.w]);
  

    //boneTransform += mul(boneWeights1.x, boneMatrix[boneIndices1.x]);
    //boneTransform += mul(boneWeights1.y, boneMatrix[boneIndices1.y]);
    //boneTransform += mul(boneWeights1.z, boneMatrix[boneIndices1.z]);
    //boneTransform += mul(boneWeights1.w, boneMatrix[boneIndices1.w]);
  
    pos = mul(pos, boneTransform);
#endif
    pos = mul(pos, world);
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
    return albedoFactor;
}
#endif
//---------------------------define OutLine--------------------------------------------
#ifdef OutLine
PS_INPUT outline_vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4x4 matWolrd;
    float scaleFactor = 0.03;
    float3 normalOffset = normalize(mul(input.normal, (float3x3) world)) * scaleFactor;
    input.position.xyz += normalOffset;
    
#ifdef Skinning
   matrix boneTransform =  matrix(
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f,
     0.f, 0.f, 0.f, 0.f
    );
    
    uint4 boneIndices0 = uint4(boneIDBuffer[input.vertexID * 8 + 0],
                               boneIDBuffer[input.vertexID * 8 + 1],
                               boneIDBuffer[input.vertexID * 8 + 2],
                               boneIDBuffer[input.vertexID * 8 + 3]);
    //uint4 boneIndices1 = uint4(boneIDBuffer[input.vertexID * 8 + 4],
    //                           boneIDBuffer[input.vertexID * 8 + 5],
    //                           boneIDBuffer[input.vertexID * 8 + 6],
    //                           boneIDBuffer[input.vertexID * 8 + 7]);

    float4 boneWeights0 = float4(boneWeightBuffer[input.vertexID * 8 + 0],
                                 boneWeightBuffer[input.vertexID * 8 + 1],
                                 boneWeightBuffer[input.vertexID * 8 + 2],
                                 boneWeightBuffer[input.vertexID * 8 + 3]);
    //float4 boneWeights1 = float4(boneWeightBuffer[input.vertexID * 8 + 4],
    //                             boneWeightBuffer[input.vertexID * 8 + 5],
    //                             boneWeightBuffer[input.vertexID * 8 + 6],
    //                             boneWeightBuffer[input.vertexID * 8 + 7]);
  
    boneTransform += mul(boneWeights0.x, boneMatrix[boneIndices0.x]);
    boneTransform += mul(boneWeights0.y, boneMatrix[boneIndices0.y]);
    boneTransform += mul(boneWeights0.z, boneMatrix[boneIndices0.z]);
    boneTransform += mul(boneWeights0.w, boneMatrix[boneIndices0.w]);
  

    //boneTransform += mul(boneWeights1.x, boneMatrix[boneIndices1.x]);
    //boneTransform += mul(boneWeights1.y, boneMatrix[boneIndices1.y]);
    //boneTransform += mul(boneWeights1.z, boneMatrix[boneIndices1.z]);
    //boneTransform += mul(boneWeights1.w, boneMatrix[boneIndices1.w]);
  
    
    matWolrd =  boneTransform;
    float4 totalPos = mul(input.position, boneTransform);
    matrix wvp = mul(world, view);
    wvp = mul(wvp, projection);
    output.position = mul(totalPos, wvp);
    output.worldPosition = mul(totalPos, world);
#else
    matWolrd = world;
    output.position = mul(input.position, matWolrd);
    output.worldPosition = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
#endif
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

float4 outline_ps_main(PS_INPUT input) :SV_Target0
{
    return albedoFactor;
}
#endif 

//---------------------------define billboard--------------------------------------------

#ifdef Billboard
QUAD_PS_INPUT billboard_vs_main(QUAD_VS_INPUT  input)
{
    QUAD_PS_INPUT output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position,projection);
    output.uv = input.uv;

    return output;
}
float4 billboard_ps_main(QUAD_PS_INPUT input) : SV_Target0
{
    return texAlbedo.Sample(samLinear, input.uv);
    //return float4(1.f, 1.f, 1.f, 1.f);
}
#endif


//---------------------------define SSAO--------------------------------------------
#ifdef SSAO

float4 ssao_normal_depth_write_ps_main(PS_INPUT input) : SV_Target0
{
    float3 N = normalize(input.worldNormal);
    float4 normalTexture = texNormal.Sample(samLinear, input.uv);
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
    float depth = input.position.z;
    return float4(N, depth);
}

float4 ssao_ao_write_ps_main(QUAD_PS_INPUT input) : SV_TARGET
{
    // Fetch Normal and Depth from GBuffer
    float4 normalData = ssaoNormalDepth.Sample(samLinear, input.uv);
    float3 normal = normalize(normalData.xyz); // Already in correct range
    float depth = normalData.w; // Depth stored in W component
    //depth = (nearPlane * farPlane) / (farPlane - depth * (farPlane - nearPlane));
    float occlusion = 0.0f;
    
    // SSAO Sampling Loop
    [unroll]
    for (int i = 0; i < 16; i++)
    {
        // Create random sampling offset
        float3 sampleDir = normalize(SSAOKernel[i]);
        float3 samplePos = float3(input.uv, depth) + sampleDir * radius;

        // Sample nearby depth values and normals
        float4 sampleNormalData = ssaoNormalDepth.Sample(samLinear, samplePos.xy);
        float3 sampleNormal = normalize(sampleNormalData.xyz);
        float sampleDepth = sampleNormalData.w;

        // Compute the angle between the current normal and the sample normal
        float3 toSampleDir = normalize(samplePos.xyz - float3(input.uv, depth)); // Vector to the sample position
        float angle = max(dot(normal, sampleNormal), 0.0f); // Dot product to compute the angle between the two normals

        // Compare depth values (occlusion test)
        float depthDifference = samplePos.z - sampleDepth;
        if (depthDifference > 0.0f && depthDifference < radius) // Within sampling range
        {
            // Add weighted occlusion based on depth difference and normal angle
            occlusion += saturate((1.0f - depthDifference / radius) * angle);
        }
    }

    // Normalize Occlusion Value
    occlusion = (occlusion / 16);

    return float4(occlusion, occlusion, occlusion, 1.0f); // Grayscale SSAO
}


#endif

#ifdef BLUR
float4 blur_horizontal_ps_main(QUAD_PS_INPUT input) : SV_Target0
{
    float weights[5] = { 0.2270f, 0.1946f, 0.1210f, 0.0545f, 0.0162f };
    float2 texelSize = float2(1.f / screenWidth, 1.f / screenHeight);
    float3 occlusion = ssaoMap.Sample(samLinear, input.uv).rgb * weights[0];
    [unroll]
    for (int i = 1; i < 5;i++)
    {
        float2 offset = float2(texelSize.x * i, 0.f);
        occlusion += ssaoMap.Sample(samLinear, input.uv + offset).rgb * weights[i];
        occlusion += ssaoMap.Sample(samLinear, input.uv - offset).rgb * weights[i];
    }
    return float4(occlusion, 1.f);
    
}

float4 blur_vertical_ps_main(QUAD_PS_INPUT input):SV_Target
{
    float weights[5] = { 0.2270f, 0.1946f, 0.1210f, 0.0545f, 0.0162f };
    float2 texelSize = float2(1.f / screenWidth, 1.f / screenHeight);
    float3 occlusion = ssaoNormalDepth.Sample(samLinear, input.uv).rgb * weights[0];

    [unroll]
    for (int i = 1; i < 5; i++)
    {
        float2 offset = float2(0.0f, texelSize.y * i); // 세로 방향 오프셋
        occlusion += ssaoNormalDepth.Sample(samLinear, input.uv + offset).rgb * weights[i];
        occlusion += ssaoNormalDepth.Sample(samLinear, input.uv - offset).rgb * weights[i];
    }

    return float4(occlusion, 1.0f);
}
#endif