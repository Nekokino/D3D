#include "GValue.fx"

Texture2D Tex_0 : register(t0);
SamplerState Smp_0 : register(s0);

struct Vtx_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

struct Vtx_Out
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

struct Pix_Out
{
    float4 Color : SV_Target;
};

Vtx_Out VS_TDBG(Vtx_In _In)
{
    Vtx_Out Out = (Vtx_Out) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;

    return Out;
}

Pix_Out PS_TDBG(Vtx_Out _In)
{
    float4 Color = Tex_0.Sample(Smp_0, _In.Uv);

    Pix_Out Out = (Pix_Out) 0.0f;
    Out.Color = Color;

    return Out;
}