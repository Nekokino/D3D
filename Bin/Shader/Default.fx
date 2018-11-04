#include "GValue.fx"

struct Default_VtxOut
{
    float4 Pos : SV_POSITION;
};

struct Default_PixOut
{
    float4 Color : SV_Target;
};

Default_VtxOut VS_Default(float4 _Pos : POSITION)
{
    Default_VtxOut Out = (Default_VtxOut) 0.0f;
    Out.Pos = mul(_Pos, WVP);
    return Out;
}

Default_PixOut PS_Default(float4 _In : SV_POSITION)
{
    Default_PixOut Out = (Default_PixOut) 0.0f;
    Out.Color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    return Out;
}