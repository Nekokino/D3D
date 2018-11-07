#include "GValue.fx"
#include "Light.fx"
#include "GTex.fx"


struct Vtx3DMesh_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT; // U와 일치하는 X축
    float4 BiNormal : BINORMAL; // V와 일치함
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

struct Pix3DMesh_Out
{
    float4 Color : SV_Target;
};

Vtx3DMesh_Out VS_Mesh3D(Vtx3DMesh_In _In)
{
    Vtx3DMesh_Out Out = (Vtx3DMesh_Out) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    Out.ViewPos = mul(_In.Normal, WV);
    Out.Normal = normalize(mul(_In.Normal, WV));
    Out.Tangent = normalize(mul(_In.Tangent, WV));
    Out.BiNormal = normalize(mul(_In.BiNormal, WV));

    if(0 == IsLight)
    {
        return Out;
    }

    if (1 == IsLightVtxOrPix)
    {
        return Out;
    }

    if (0 == LightCount)
    {
        return Out;
    }

    LightColor LC;
    LC.Ambient = (float4)0.0f;
    LC.Diffuse = (float4) 0.0f;
    LC.Specular = (float4) 0.0f;

    for (int i = 0; i < LightCount; ++i)
    {
        LightColor TmpCol = CalDirLight(Out.ViewPos, Out.Normal, Arr[i]);
        LC.Diffuse += TmpCol.Diffuse;
        LC.Specular += TmpCol.Specular;
        LC.Ambient += TmpCol.Ambient;
    }

    LC.Diffuse /= (float)LightCount;
    LC.Specular /= (float)LightCount;
    LC.Ambient /= (float)LightCount;

    Out.Color.rgb = _In.Color.rgb * LC.Diffuse.rgb + LC.Specular.rgb + LC.Ambient.rgb;
    Out.Color.a = _In.Color.a;

    return Out;
}

Pix3DMesh_Out PS_Mesh3D(Vtx3DMesh_Out _In)
{
    Pix3DMesh_Out Out = (Pix3DMesh_Out) 0.0f;

    Out.Color = _In.Color;

    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < TexCount; ++i)
    {
        if (-1 != TexArr[i].Tex_Idx)
        {
            if (TexArr[i].Type == TEX)
            {
                Color *= GetTexColor(TexArr[i].Tex_Idx, TexArr[i].Tex_Smp, _In.Uv) * _In.Color;
            }
            else if (TexArr[i].Type == BUMP)
            {
                _In.Normal = CalBump(TexArr[i].Tex_Idx, TexArr[i].Tex_Smp, _In.Uv, _In.Tangent, _In.BiNormal, _In.Normal);
            }
        }
    }

   

    if (0 == IsLight)
    {
        return Out;
    }

    if (0 == IsLightVtxOrPix)
    {
        return Out;
    }

    if (0 == LightCount)
    {
        return Out;
    }

    LightColor LC;
    LC.Ambient = (float4) 0.0f;
    LC.Diffuse = (float4) 0.0f;
    LC.Specular = (float4) 0.0f;

    for (int j = 0; j < LightCount; ++j)
    {
        LightColor TmpCol = CalDirLight(_In.ViewPos, _In.Normal, Arr[j]);
        LC.Diffuse += TmpCol.Diffuse;
        LC.Specular += TmpCol.Specular;
        LC.Ambient += TmpCol.Ambient;
    }

    LC.Diffuse /= (float) LightCount;
    LC.Specular /= (float) LightCount;
    LC.Ambient /= (float) LightCount;

    Out.Color.rgb = Color.rgb * LC.Diffuse.rgb + LC.Specular.rgb + LC.Ambient.rgb;
    Out.Color.a = _In.Color.a;

    return Out;
}