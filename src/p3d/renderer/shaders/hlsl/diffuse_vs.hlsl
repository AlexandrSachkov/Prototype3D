struct VS_INPUT
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 WVP;
};

VS_OUTPUT main(in VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(WVP, float4(input.pos, 1.0f));
    //output.pos = mul(float4(input.pos, 1.0f), WVP);
    //output.pos = float4(input.pos, 1.0f);
	output.uv = input.uv;

	return output;
}