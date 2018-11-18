#include "GValue.fx"
#include "PixValue.fx"
#include "DefferdLight.fx"
#include "BoneAni.fx"

struct Vtx3DMesh_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
    float4 Weights : WEIGHTS;
    float4 Indices : BLENDINDICES;
};

struct Vtx3DMesh_Out
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};



Vtx3DMesh_Out VS_DefferdAni(Vtx3DMesh_In _In)
{
    Vtx3DMesh_Out Out = (Vtx3DMesh_Out) 0.0f;

    Skinning(_In.Pos, _In.Tangent, _In.BiNormal, _In.Normal, _In.Weights, _In.Indices, 0);


    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    Out.ViewPos = mul(_In.Pos, WV);
    Out.Normal = normalize(mul(_In.Normal, WV));
    Out.Tangent = normalize(mul(_In.Tangent, WV));
    Out.BiNormal = normalize(mul(_In.BiNormal, WV));

    return Out;
}

PS_DefferdOut PS_DefferdAni(Vtx3DMesh_Out _In)
{
    PS_DefferdOut Out = (PS_DefferdOut) 0.0f;
    Out.Diffuse = _In.Color;
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < TexCount; ++i)
    {
        if (-1 != TexArr[i].Tex_Idx)
        {
            if (TexArr[i].Type == TEX)
            {
                Color = GetTexColor(TexArr[i].Tex_Idx, TexArr[i].Tex_Smp, _In.Uv) * _In.Color;
            }
            else if (TexArr[i].Type == BUMP)
            {
                _In.Normal = CalBump(TexArr[i].Tex_Idx, TexArr[i].Tex_Smp, _In.Uv, _In.Tangent, _In.BiNormal, _In.Normal);
            }
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