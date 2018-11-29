#include "GValue.fx"
#include "PixValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

cbuffer TerrainBuffer : register(b0)
{
    int FloorCount;
    int VtxX;
    int VtxZ;
    int TextureCount;
    int IsBump[4];
}

struct Vtx_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};

struct Vtx_Out
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};

Vtx_Out VS_TerrainDefferd(Vtx_In _In)
{
    Vtx_Out Out = (Vtx_Out) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    Out.ViewPos = mul(_In.Pos, WV);
    Out.Normal = normalize(mul(_In.Normal, WV));
    Out.Tangent = normalize(mul(_In.Tangent, WV));
    Out.BiNormal = normalize(mul(_In.BiNormal, WV));
    
    return Out;
}

PS_DefferdOut PS_TerrainDefferd(Vtx_Out _In)
{
    PS_DefferdOut Out = (PS_DefferdOut) 0.0f;
    Out.Diffuse = _In.Color;
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    Color *= GetMultiTexColor(8, 8, _In.Uv, 0.0f);
    _In.Normal = CalMultiBump(8, 8, _In.Uv, 1.0f, _In.Tangent, _In.BiNormal, _In.Normal);

    float2 SpUv;

    SpUv.x = _In.Uv.x / VtxX;
    SpUv.y = _In.Uv.y / VtxZ;

    for (int i = 0; i < FloorCount; ++i)
    {
        float4 Ratio = GetTexColor(i, i, SpUv);
        float RatioValue = (Ratio.x + Ratio.y + Ratio.z) / 3.0f;
        float4 FloorColor = GetMultiTexColor(9 + i, 9 + i, _In.Uv, 0.0f);
        float4 SrcColor = Color;

        FloorColor.xyz *= RatioValue;
        SrcColor.xyz *= (1.0f - Ratio.x);
        Color = FloorColor + SrcColor;

        if (RatioValue >= 0.9f)
        {
            _In.Normal = CalMultiBump(9 + i, 9 + i, _In.Uv, 1.0f, _In.Tangent, _In.BiNormal, _In.Normal);
        }
    }

    Out.Diffuse.rgb = Color;
    Out.Diffuse.a = _In.Color.a;
    Out.Normal = _In.Normal;
    Out.Normal.a = 1.0f;
    Out.Position = _In.ViewPos;
    Out.Depth.x = Out.Position.z;
    Out.Depth.w = 1.0f;

    return Out;
}