cbuffer cbTransform : register(b0)
{
	float4x4 world;
	float4x4 worldInvTrans;
	float4x4 wvp;
};

float4 main( float3 pos : POSITION ) : SV_POSITION {
	float4 final = mul(wvp, float4(pos, 1.0f));
	return final;
}