struct INPUT
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

cbuffer Material : register(b0) {
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;
	float4 transparentColor;
	float4 emissiveColor;

	float shininess;
	float shininessStr;
	float opacity;
	float _b;

	bool hasDiffuseTex;
	bool hasAmbientTex;
	bool hasSpecularTex;
	bool hasNormalTex;
	bool hasShininessTex;
	bool hasOpacityTex;
	bool hasEmissionTex;
}

Texture2D diffuseTex	: register(t0);
Texture2D ambientTex	: register(t1);
Texture2D specularTex	: register(t2);
Texture2D normalTex		: register(t3);
Texture2D shininessTex	: register(t4);
Texture2D opacityTex	: register(t5);
Texture2D emissionTex	: register(t6);

SamplerState diffuseSS		: register(s0);
SamplerState ambientSS		: register(s1);
SamplerState specularSS		: register(s2);
SamplerState normalSS		: register(s3);
SamplerState shininessSS	: register(s4);
SamplerState opacitySS		: register(s5);
SamplerState emissionSS		: register(s6);

float4 main(in INPUT input) : SV_TARGET
{
	float4 diffuseTexColor = diffuseTex.Sample(diffuseSS, input.texCoord);
	return diffuseTexColor;
}