cbuffer cbTransform : register(b0)
{
	float4x4 world;
	float4x4 worldInvTrans;
	float4x4 wvp;
};

struct INPUT
{
	float3 pos : POSITION;
	float3 normal: NORMAL;
	float2 texCoord : TEXCOORD;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

OUTPUT main( in INPUT input )
{
	OUTPUT output;
	output.pos = mul(wvp, float4(input.pos, 1.0f));
	output.posW = input.pos;//wrong, needs to be changed
	output.normalW = input.normal;//wrong, needs to be changed
	output.texCoord = float2(input.texCoord.x, 1 - input.texCoord.y); //adjusts for DX convention

	return output;
}