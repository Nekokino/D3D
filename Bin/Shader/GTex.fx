#define TEX 0
#define BUMP 1

Texture2D Tex_0 : register(t0);
Texture2D Tex_1 : register(t1);
Texture2D Tex_2 : register(t2);
Texture2D Tex_3 : register(t3);
Texture2D Tex_4 : register(t4);
Texture2D Tex_5 : register(t5);
Texture2D Tex_6 : register(t6);
Texture2D Tex_7 : register(t7);

Texture2DArray TexArr_0 : register(t8);
Texture2DArray TexArr_1 : register(t9);
Texture2DArray TexArr_2 : register(t10);
Texture2DArray TexArr_3 : register(t11);
Texture2DArray TexArr_4 : register(t12);
Texture2DArray TexArr_5 : register(t13);
Texture2DArray TexArr_6 : register(t14);
Texture2DArray TexArr_7 : register(t15);

SamplerState Sam_0 : register(s0);
SamplerState Sam_1 : register(s1);
SamplerState Sam_2 : register(s2);
SamplerState Sam_3 : register(s3);
SamplerState Sam_4 : register(s4);
SamplerState Sam_5 : register(s5);
SamplerState Sam_6 : register(s6);
SamplerState Sam_7 : register(s7);
SamplerState Sam_8 : register(s8);
SamplerState Sam_9 : register(s9);
SamplerState Sam_10 : register(s10);
SamplerState Sam_11 : register(s11);
SamplerState Sam_12 : register(s12);
SamplerState Sam_13 : register(s13);
SamplerState Sam_14 : register(s14);
SamplerState Sam_15 : register(s15);


float4 GetColor(Texture2D _Tex, int _Samp, float2 _Uv)
{
    switch (_Samp)
    {
        case 0:
            return _Tex.Sample(Sam_0, _Uv);
        case 1:
            return _Tex.Sample(Sam_1, _Uv);
        case 2:
            return _Tex.Sample(Sam_2, _Uv);
        case 3:
            return _Tex.Sample(Sam_3, _Uv);
        case 4:
            return _Tex.Sample(Sam_4, _Uv);
        case 5:
            return _Tex.Sample(Sam_5, _Uv);
        case 6:
            return _Tex.Sample(Sam_6, _Uv);
        case 7:
            return _Tex.Sample(Sam_7, _Uv);
        default:
            return (float4) 0.0f;

    }
}

float4 GetTexColor(int _Tex, int _Samp, float2 _Uv)
{
    switch (_Tex)
    {
        case 0:
            return GetColor(Tex_0, _Samp, _Uv);
        case 1:
            return GetColor(Tex_1, _Samp, _Uv);
        case 2:
            return GetColor(Tex_2, _Samp, _Uv);
        case 3:
            return GetColor(Tex_3, _Samp, _Uv);
        case 4:
            return GetColor(Tex_4, _Samp, _Uv);
        case 5:
            return GetColor(Tex_5, _Samp, _Uv);
        case 6:
            return GetColor(Tex_6, _Samp, _Uv);
        case 7:
            return GetColor(Tex_7, _Samp, _Uv);
        default:
            return (float4) 0.0f;

    }
}

float4 GetMultiColor(Texture2DArray _Tex, int _Smp, float2 _Uv, float _Idx)
{
    float3 UV3D;

    UV3D.xy = _Uv.xy;
    UV3D.z = _Idx;

    switch (_Smp)
    {
        case 8:
            return _Tex.Sample(Sam_8, UV3D);
        case 9:
            return _Tex.Sample(Sam_9, UV3D);
        case 10:
            return _Tex.Sample(Sam_10, UV3D);
        case 11:
            return _Tex.Sample(Sam_11, UV3D);
        case 12:
            return _Tex.Sample(Sam_12, UV3D);
        case 13:
            return _Tex.Sample(Sam_13, UV3D);
        case 14:
            return _Tex.Sample(Sam_14, UV3D);
        case 15:
            return _Tex.Sample(Sam_15, UV3D);
        default:
            return (float4) 0.0f;

    }
}

float4 GetMultiTexColor(int _Tex, int _Smp, float2 _Uv, float _Idx)
{
    switch (_Tex)
    {
        case 8:
            return GetMultiColor(TexArr_0, _Smp, _Uv, _Idx);
        case 9:
            return GetMultiColor(TexArr_1, _Smp, _Uv, _Idx);
        case 10:
            return GetMultiColor(TexArr_2, _Smp, _Uv, _Idx);
        case 11:
            return GetMultiColor(TexArr_3, _Smp, _Uv, _Idx);
        case 12:
            return GetMultiColor(TexArr_4, _Smp, _Uv, _Idx);
        case 13:
            return GetMultiColor(TexArr_5, _Smp, _Uv, _Idx);
        case 14:
            return GetMultiColor(TexArr_6, _Smp, _Uv, _Idx);
        case 15:
            return GetMultiColor(TexArr_7, _Smp, _Uv, _Idx);
        default:
            return (float4) 0.0f;

    }
}

float4 CalBump(int _BumpIdx, int _BumpSmp, float2 _Uv, float4 _ViewTangent, float4 _ViewBiNormal, float4 _Normal)
{
    float4 BumpNormal = GetTexColor(_BumpIdx, _BumpSmp, _Uv);

    BumpNormal = BumpNormal * 2.0f - 1.0f;
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;

    float3x3 MatTBN = float3x3(_ViewTangent.xyz, _ViewBiNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, MatTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}

float4 CalMultiBump(int _BumpIdx, int _BumpSmp, float2 _Uv, float _Idx, float4 _ViewTangent, float4 _ViewBiNormal, float4 _Normal)
{
    float4 BumpNormal = GetMultiTexColor(_BumpIdx, _BumpSmp, _Uv, _Idx);

    BumpNormal = BumpNormal * 2.0f - 1.0f;
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;

    float3x3 MatTBN = float3x3(_ViewTangent.xyz, _ViewBiNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, MatTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}