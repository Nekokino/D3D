#include "Light.fx"
#include "GTex.fx"

struct Vtx3DMesh_In
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct Vtx3DMesh_Out
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL;
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
    Out.ViewPos = mul(mul(_In.Normal, World), View);
    _In.Normal = 0.0f;
    Out.Normal = normalize(mul(mul(_In.Normal, World), View));

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

    for (int i = 0; i < LightCount; ++i)
    {
        LightColor TmpCol = CalDirLight(_In.ViewPos, _In.Normal, Arr[i]);
        LC.Diffuse += TmpCol.Diffuse;
        LC.Specular += TmpCol.Specular;
        LC.Ambient += TmpCol.Ambient;
    }

    LC.Diffuse /= (float) LightCount;
    LC.Specular /= (float) LightCount;
    LC.Ambient /= (float) LightCount;

    Out.Color.rgb = _In.Color.rgb * LC.Diffuse.rgb + LC.Specular.rgb + LC.Ambient.rgb;
    Out.Color.a = _In.Color.a;

    return Out;
}