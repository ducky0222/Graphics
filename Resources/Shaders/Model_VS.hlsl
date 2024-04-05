struct VS_INPUT
{
	float3 PosL : Position;
	float2 UV : Texcoord;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
};

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 UV : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 T : TEXCOORD2;
	float4 ShadowPosH : TEXCOORD3;
	float4 SsaoPosH : TEXCOORD4;
};

cbuffer cbTransformW : register(b0)
{
	matrix gWorld;
	matrix gView;
	matrix gProj;
	matrix gShadowView;
	matrix gShadowProj;
	matrix gTexMatrix;
};

static const matrix toTextureSpace =
{
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.0f, 1.0f
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.PosW = mul(float4(Input.PosL, 1.f), gWorld).xyz;

	Output.PosH = mul(float4(Input.PosL, 1.f), gWorld);
	Output.PosH = mul(Output.PosH, gView);
	Output.PosH = mul(Output.PosH, gProj);

	Output.ShadowPosH = mul(float4(Input.PosL, 1.f), gWorld);
	Output.ShadowPosH = mul(Output.ShadowPosH, gShadowView);
	Output.ShadowPosH = mul(Output.ShadowPosH, gShadowProj);
	Output.ShadowPosH = mul(Output.ShadowPosH, toTextureSpace);

	Output.SsaoPosH = mul(Output.PosH, toTextureSpace);

	Output.UV = mul(float4(Input.UV, 0.0f, 1.0f), gTexMatrix).xy;

	float3 worldNormal = mul(Input.Normal, (float3x3) gWorld);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.Tangent, (float3x3) gWorld);
	Output.T = normalize(worldTangent);

	return Output;
}
