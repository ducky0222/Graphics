#include "LightHelper.hlsli"

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

// lightCB
cbuffer cbFrame : register(b0)
{
	DirectionLight gDirLights[3];
	int gLightCount;
	float3 gEyePosW;

	float4 gFogColor;
	float gFogStart;
	float gFogRange;
	bool gUseFog;
	bool gUseIBL;
	bool gUseSsaoMap;
};

cbuffer cbMapUsed : register(b1)
{
	bool gUseDiffuse;
	bool gUseNormal;
	bool gUseSpecular;
	bool gUseAlphaMap;
}

cbuffer cbAlphaUsed : register(b2)
{
	bool gUseAlpha;
	float gAlpha;
}

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);
Texture2D gAlphaMap : register(t3);
Texture2D gShadowMap : register(t9);
Texture2D gSsaoMap : register(t10);

SamplerComparisonState gSamShadow : register(s0);
SamplerState gSamplerLinear : register(s1);
SamplerState gSamplerLinear2 : register(s2);

float4 main(VS_OUTPUT Input) : SV_TARGET
{
    float opacity = 1.f;
	
    if (gUseAlphaMap)
    {
        opacity = gAlphaMap.Sample(gSamplerLinear, Input.UV).a;
        clip(opacity - 0.1f);
    }
    if (gUseAlpha)
    {
        opacity = min(gAlpha, opacity);
        clip(gAlpha);
    }
   
	float3 toEye = gEyePosW - Input.PosW;
	float distanceEye = length(toEye);
	toEye /= distanceEye;

	float3 normal = normalize(Input.N);

	if (gUseNormal)
	{
		normal = gNormalMap.Sample(gSamplerLinear, Input.UV).xyz;
		normal = NormalSampleToWorldSpace(normal, Input.N, Input.T);
	}

	// 요 부분 어찌 다룰지 조금 더 생각해보자
	Material material;
	material.Specular = float4(0.5f, 0.5f, 0.5f, 8.f);
	material.Diffuse = float4(0.5f, 0.5f, 0.5f, 1.f);
	material.Ambient = float4(0.1f, 0.1f, 0.1f, 0.1f);

	if (gUseDiffuse)
	{
		material.Diffuse = gDiffuseMap.Sample(gSamplerLinear, Input.UV);
		// clip(material.Diffuse.a - 0.1f);
	}

	if (gUseSpecular)
	{
		material.Specular = gSpecularMap.Sample(gSamplerLinear, Input.UV);
	}

	float4 litColor = 1.f;

	if (gLightCount > 0)
	{
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		float3 shadow = float3(1.f, 1.f, 1.f);
		shadow[0] = CalcShadowFactor(gSamShadow, gShadowMap, Input.ShadowPosH);

		float ambientAccess = 1.f;

		//if (gUseSsaoMap)
		//{
		//	Input.SsaoPosH /= Input.SsaoPosH.w;
		//	ambientAccess = gSsaoMap.SampleLevel(gSamplerLinear, Input.SsaoPosH.xy, 0.0f).r;
		//}

		[unroll]
		for (int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionLight(material, gDirLights[i], normal, toEye, A, D, S);

			ambient += A * ambientAccess;
			diffuse += D * shadow[i];
			spec += S * shadow[i];
		}

		litColor = ambient + diffuse + spec;
	}

	if (gUseFog)
	{
		float fogLerp = saturate((distanceEye - gFogStart) / gFogRange);
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

    litColor.a = opacity;
	litColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return litColor;
}
