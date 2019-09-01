struct VS_INPUT
{
	float3 pos : POSITION;
    float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float3 posW : POSITION;
    float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
    float4 pos : SV_POSITION;
};

cbuffer Transforms : register(b0)
{
    float4x4 world;
    float4x4 worldInvTrans;
    float4x4 wvp;
};

VS_OUTPUT main(in VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(wvp, float4(input.pos, 1.0f));
    output.posW = mul(world, float4(input.pos, 1.0f)).xyz;
    output.normalW = mul((float3x3)worldInvTrans, input.normal);
	output.uv = input.uv;

	return output;
}