struct PS_INPUT
{
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer SceneConstants : register(b0) 
{
    float3 ambientLight;
};

cbuffer Material : register(b1)
{
    float3 diffuseColor;
    float shininess;
    float3 specularColor;
    float shininessStrength;
    float3 transparencyColor;
    float reflectivity;
    float3 reflectionColor;
    float refracti;
    float3 emissionColor;
    float opacity;

    int hasDiffuteTex;
    int hasEmissionTex;
    int hasLightmapTex;
    int hasNormalTex;
    int hasOpacityTex;
    int hasReflectionTex;
    int hasShininessTex;
    int hasSpecularTex;
};

Texture2D<float4> diffuseTex : register(t0);
Texture2D<float4> emissionTex : register(t1);
Texture2D<float4> lightmapTex : register(t2);
Texture2D<float4> normalTex : register(t3);
Texture2D<float4> opacityTex : register(t4);
Texture2D<float4> reflectionTex : register(t5);
Texture2D<float4> shininessTex : register(t6);
Texture2D<float4> specularTex : register(t7);

SamplerState samplerState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uvFlipped = float2(input.uv.x, -input.uv.y);//flip from OpenGL standard

    float4 diffuseColorTex = float4(diffuseColor, opacity);
    if (hasDiffuteTex) {
        diffuseColorTex = diffuseTex.Sample(samplerState, uvFlipped);
    }
    
    if (hasOpacityTex) {
        diffuseColorTex.a = opacityTex.Sample(samplerState, uvFlipped).r;
    }
    
    clip(diffuseColorTex.a < 0.05f ? true : false);

    float4 specularColorTex = float4(specularColor, 1.0f);
    if (hasSpecularTex) {
        specularColorTex = specularTex.Sample(samplerState, uvFlipped);
    }

    float4 totalLightIntensity = float4(0.0f, 0.0f, 0.0f, 0.0f);//TODO calculate light contribution

    float4 litColor = 
        float4(ambientLight, 1.0f) * diffuseColorTex +
        totalLightIntensity * diffuseColorTex + 
        totalLightIntensity * specularColorTex;

    return litColor;
}