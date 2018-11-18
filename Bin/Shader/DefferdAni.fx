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

class SkinInfo
{
    float4 Pos;
    float4 Normal;
    float4 BiNormal;
    float4 Tangent;
};

matrix GetBoneMat(int _BoneIdx, int _RowIdx)
{
    // Load => 그냥 픽셀단위로 색을 가져온다 (샘플러를 통하지 않고)

    matrix BoneMat =
    {
        BoneTex.Load(int3((_BoneIdx * 4), _RowIdx, 0)),
        BoneTex.Load(int3((_BoneIdx * 4) + 1, _RowIdx, 0)),
        BoneTex.Load(int3((_BoneIdx * 4) + 2, _RowIdx, 0)),
        BoneTex.Load(int3((_BoneIdx * 4) + 3, _RowIdx, 0))
    };


    return BoneMat;
}

void Skinning(inout float4 _Pos, inout float4 _Tangent, inout float4 _BiNormal, inout float4 _Normal, inout float4 _Weights, inout float4 _Indices, int _RowIdx)
{
    SkinInfo Info = (SkinInfo) 0.0f;
    Info.Pos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Info.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Info.BiNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Info.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        if (0.0f == _Weights[i])
        {
            continue;
        }

        matrix MatBone = GetBoneMat((int) _Indices[i], _RowIdx);

        Info.Pos += mul(_Pos, MatBone) * _Weights[i];
        Info.Tangent += mul(_Tangent, MatBone) * _Weights[i];
        Info.BiNormal += mul(_BiNormal, MatBone) * _Weights[i];
        Info.Normal += mul(_Normal, MatBone) * _Weights[i];
    }

    Info.Pos.w = 1.0f;
    Info.Tangent.w = 0.0f;
    Info.BiNormal.w = 0.0f;
    Info.Normal.w = 0.0f;

    _Pos = Info.Pos;
    _Tangent = normalize(Info.Tangent);
    _BiNormal = normalize(Info.BiNormal);
    _Normal = normalize(Info.Normal);

}

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