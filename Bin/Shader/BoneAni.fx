#include "GTex.fx"

Texture2D BoneTex : register(t10);

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