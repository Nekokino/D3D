#include "Light.fx"

cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WVP;
}

struct PixLightVtx_Input
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct PixLightVtx_Output
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct PixLightPix_Output
{
    float4 Color : SV_Target;
};

PixLightVtx_Output VS_PixLight(PixLightVtx_Input _In)
{
    PixLightVtx_Output Out = (PixLightVtx_Output) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color = _In.Color;
    Out.Normal = _In.Normal;

    return Out;
}

PixLightPix_Output PS_PixLight(PixLightVtx_Output _In)
{
    PixLightPix_Output Out = (PixLightPix_Output) 0.0f;
    Out.Color = _In.Color;
    return Out;
}