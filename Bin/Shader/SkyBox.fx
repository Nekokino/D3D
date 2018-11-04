#include "GValue.fx"

Texture2D Tex_0 : register(t0);
SamplerState Sam_0 : register(s0);

cbuffer MulColor : register(b0)
{
    float4 Color;
}

struct VTXSKY_IN
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct VTXSKY_OUT
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct PIXSKY_OUT
{
    float4 Color : SV_Target;
};

VTXSKY_OUT VS_SkyBox(VTXSKY_IN _In)
{
    VTXSKY_OUT Out = (VTXSKY_OUT) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color = _In.Color;
    Out.Normal = _In.Normal;

    return Out;
}

PIXSKY_OUT PS_SkyBox(VTXSKY_OUT _In)
{
    PIXSKY_OUT Out = (PIXSKY_OUT) 0.0f;
    Out.Color = Tex_0.Sample(Sam_0, float2(_In.Uv.x, _In.Uv.y));
    return Out;
}