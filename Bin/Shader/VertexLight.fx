#include "Light.fx"

cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WVP;
}

struct VtxLightVtx_Input
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
};

struct VtxLightVtx_Output
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 ViewPos : POSITION;
    float4 Normal : NORMAL;
};

struct VtxLightPix_Output
{
    float4 Color : SV_Target;
};

VtxLightVtx_Output VS_VtxLight(VtxLightVtx_Input _In)
{
    VtxLightVtx_Output Out = (VtxLightVtx_Output) 0.0f;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    Out.ViewPos = mul(mul(_In.Pos, World), View);
    Out.Normal = normalize(mul(mul(_In.Normal, World), View));

    LightData LD;
    LD.Range = 500.0f;
    LD.Color.Diffuse = float4(1.0f, 1.0f, 1.0f, 0.0f);
    LD.Color.Specular = float4(0.2f, 0.2f, 0.2f, 0.0f);
    LD.Color.Ambient = float4(0.1f, 0.1f, 0.1f, 0.0f);

    LD.Pos = float4(200.0f, 200.0f, 500.0f, 0.0f);
    LD.Pos = -mul(LD.Pos, View);

    LightData Call = (LightData)0.0f;

    float4 Light = -mul(LD.Pos - Out.ViewPos, World);

    Call.Color.Diffuse = LD.Color.Diffuse * saturate(dot(Out.Normal, normalize(Light)));

    Light = normalize(Light);

    float4 Reflect = normalize(2.0f * dot(Light, Out.Normal) * Out.Normal - Light);
    float4 Eye = normalize(-Out.ViewPos);

    Call.Color.Specular = LD.Color.Specular * pow(saturate(dot(Eye, Reflect)), 10);
    Call.Color.Ambient = LD.Color.Ambient;

    Out.Color.rgb = _In.Color.rgb * Call.Color.Diffuse.rgb + Call.Color.Specular.rgb, Call.Color.Ambient;
    Out.Color.a = _In.Color.a;
    return Out;
}

VtxLightPix_Output PS_VtxLight(VtxLightVtx_Output _In)
{
    VtxLightPix_Output Out = (VtxLightPix_Output) 0.0f;
    Out.Color = _In.Color;
    return Out;
}