struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D<float4> diffuse : register(t0);
SamplerState diffuseSamplerState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	return diffuse.Sample(diffuseSamplerState, input.uv);
}