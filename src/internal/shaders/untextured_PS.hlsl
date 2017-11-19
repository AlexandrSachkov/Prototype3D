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

float4 main() : SV_TARGET {
	return diffuseColor;
}