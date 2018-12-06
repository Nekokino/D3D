#include "GValue.fx"
#include "PixValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

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

Vtx_Out VS_Defferd(Vtx_In _In)
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

PS_DefferdOut PS_Defferd(Vtx_Out _In)
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

static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f }
};

VS_DefferdLightOut VS_DefferdLight(VS_DefferdLightIn _In)
{
    VS_DefferdLightOut Out = (VS_DefferdLightOut) 0.0f;
    Out.Pos = mul(_In.Pos, DirMat);
    Out.Uv = _In.Uv;
    return Out;
}

PS_DefferdLightOut PS_DefferdLight(VS_DefferdLightOut _In)
{
    PS_DefferdLightOut Out = (PS_DefferdLightOut) 0.0f;

    float Depth = Tex_2.Sample(Sam_0, _In.Uv).x;

    if (Depth <= 0.01f)
    {
        clip(-1);
    }

    float4 ViewPos = Tex_0.Sample(Sam_0, _In.Uv);
    float4 Normal = Tex_1.Sample(Sam_0, _In.Uv);

    LightColor ColorInfo = CalDirLight(ViewPos, Normal, LData);

    Out.Diffuse.rgb = ColorInfo.Diffuse.rgb;
    Out.Diffuse.a = 1.0f;
    Out.Specular.rgb = ColorInfo.Specular.rgb + float3(0.1f, 0.1f, 0.1f);
    Out.Specular.a = 1.0f;

    return Out;
}

VS_DefferdLightOut VS_DefferdMerge(VS_DefferdLightIn _In)
{
    VS_DefferdLightOut Out = (VS_DefferdLightOut) 0.0f;

    Out.Pos = mul(_In.Pos, DirMat);
    Out.Uv = _In.Uv;
    return Out;
}

PS_MergeOut PS_DefferdMerge(VS_DefferdLightOut _In)
{
    PS_MergeOut Out = (PS_MergeOut) 0.0f;

    float4 Color = Tex_0.Sample(Sam_0, _In.Uv);
    float4 Diffuse = Tex_1.Sample(Sam_0, _In.Uv);
    float4 Specular = Tex_2.Sample(Sam_0, _In.Uv);

    Color.w = 1.0f;
    Diffuse.w = 1.0f;
    Specular.w = 0.0f;

    //if (Out.MergeColor.a != 0.0f)
    //{
    //    Out.MergeColor.rgb = float3(0.2f, 0.2f, 0.8f) * Color.a;
    //}


    Out.MergeColor.rgb += Color.rgb * Diffuse.rgb + Specular.rgb;
    Out.MergeColor.a = 1.0f;

    return Out;
}