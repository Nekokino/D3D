#include "GValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

struct VS_VolumeOut
{
    float4 Pos : SV_POSITION;
};

struct PS_VolumeOut
{
    float4 Color : SV_Target;
};

VS_VolumeOut VS_Volume(float4 _Pos : POSITION)
{
    VS_VolumeOut Out = (VS_VolumeOut) 0.0f;
    Out.Pos = mul(_Pos, WVP);
    return Out;
}

PS_VolumeOut PS_Volume(VS_VolumeOut _In)
{
    PS_VolumeOut Out = (PS_VolumeOut) 0.0f;

    return Out;
}