#include "Light.fx"

Texture2D Tex_0 : register(t0);
SamplerState Samp_0 : register(s0);

struct VTX3D_INPUT
{
	float4 Pos : POSITION;
	float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct VTX3D_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct PIX3D_OUTPUT
{
    float4 Color : SV_Target;
};

VTX3D_OUTPUT VS_Rect3D(VTX3D_INPUT _In)
{
    VTX3D_OUTPUT OutData = (VTX3D_OUTPUT)0.0f;

    OutData.Pos = mul(_In.Pos, WVP);
    OutData.Uv = _In.Uv;
    OutData.Color = _In.Color;
    OutData.Normal = _In.Normal;

    return OutData;
}

PIX3D_OUTPUT PS_Rect3D(VTX3D_OUTPUT _In)
{
    PIX3D_OUTPUT OutData = (PIX3D_OUTPUT)0.0f;
    OutData.Color = _In.Color;
    return OutData;
}