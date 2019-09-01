struct PS_INPUT
{
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer Material : register(b0)
{
    float3 ambientColor;
    float opacity;
    float3 diffuseColor;
    float shininess;
    float3 specularColor;
    float shininessStrength;
    float3 transparencyColor;
    float reflectivity;
    float3 reflectionColor;
    float refracti;
    float3 emissionColor;

    bool hasAmbientTex = false;
    bool hasDiffuteTex = false;
    bool hasEmissionTex = false;
    bool hasLightmapTex = false;
    bool hasNormalTex = false;
    bool hasOpacityTex = false;
    bool hasReflectionTex = false;
    bool hasShininessTex = false;
    bool hasSpecularTex = false;
};

Texture2D<float4> ambientTex : register(t0);
Texture2D<float4> diffuseTex : register(t1);
Texture2D<float4> emissionTex : register(t2);
Texture2D<float4> lightmapTex : register(t3);
Texture2D<float4> normalTex : register(t4);
Texture2D<float4> opacityTex : register(t5);
Texture2D<float4> reflectionTex : register(t6);
Texture2D<float4> shininessTex : register(t7);
Texture2D<float4> specularTex : register(t8);

SamplerState samplerState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	return diffuseTex.Sample(samplerState, float2(input.uv.x, -input.uv.y));//flip from OpenGL standard
}