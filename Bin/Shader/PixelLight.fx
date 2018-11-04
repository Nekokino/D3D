#include "Light.fx"

struct PixLightVtx_Input
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct PixLightVtx_Output
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL;
};

struct PixLightPix_Output
{
    float4 Color : SV_Target;
};

PixLightVtx_Output VS_PixLight(PixLightVtx_Input _In)
{
    PixLightVtx_Output Out = (PixLightVtx_Output) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    Out.ViewPos = mul(mul(_In.Pos, World), View);
    Out.Normal = normalize(mul(mul(_In.Normal, World), View));

    return Out;
}

PixLightPix_Output PS_PixLight(PixLightVtx_Output _In)
{
    PixLightPix_Output Out = (PixLightPix_Output) 0.0f;
    
    LightData LD;
    LD.Range = 500.0f;
    LD.Color.Diffuse = float4(1.0f, 1.0f, 1.0f, 0.0f);
    LD.Color.Specular = float4(0.2f, 0.2f, 0.2f, 0.0f);
    LD.Color.Ambient = float4(0.1f, 0.1f, 0.1f, 0.0f);

    LD.Dir = float4(1.0f, 0.0f, 0.0f, 0.0f);
    LD.Dir = normalize(LD.Dir);
    LD.Pos = float4(200.0f, 200.0f, 500.0f, 0.0f);
    LD.Pos = -mul(LD.Pos, View);

    LightColor LC = CalDirLight(_In.ViewPos, _In.Normal, LD);

    Out.Color.rgb = _In.Color.rgb * LC.Diffuse.rgb + LC.Specular.rgb + LC.Ambient;
    Out.Color.a = _In.Color.a;

    return Out;
}