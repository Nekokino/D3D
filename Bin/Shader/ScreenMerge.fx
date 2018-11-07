#include "GValue.fx"
#include "PixValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

struct VS_ScreenMerge_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

struct VS_ScreenMerge_Out
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f }
};

VS_ScreenMerge_Out VS_ScreenMerge(VS_ScreenMerge_In _In)
{
    VS_ScreenMerge_Out Out = (VS_ScreenMerge_Out) 0.0f;
    Out.Pos = mul(_In.Pos, DirMat);
    Out.Uv = _In.Uv;
    return Out;
}

PS_MergeOut PS_ScreenMerge(VS_ScreenMerge_Out _In)
{
    PS_MergeOut Out = (PS_MergeOut) 0.0f;
    Out.MergeColor = Tex_0.Sample(Sam_0, _In.Uv);
    return Out;
}