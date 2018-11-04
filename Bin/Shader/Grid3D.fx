#include "GValue.fx"

struct VTX_OUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
};

struct PIX_OUT
{
    float4 Color : SV_Target;
};

VTX_OUT VS_Grid3D(float4 Pos : POSITION)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;

    Out.Pos = mul(Pos, WVP);
    Out.WorldPos = mul(Pos, World);

    return Out;
}

cbuffer GridData : register(b0)
{
    float4 GData;
}

PIX_OUT PS_Grid3D(VTX_OUT _In)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;

    float BlockSize = GData.x;
    float Alpha = GData.z;
    float Border = GData.y;
   
    float X = abs(_In.WorldPos.x % (BlockSize * 10));
    float Z = abs(_In.WorldPos.z % (BlockSize * 10));

    if (X <= Border || Z <= Border)
    {
        Out.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
        return Out;
    }

    X = abs(_In.WorldPos.x % (BlockSize));
    Z = abs(_In.WorldPos.z % (BlockSize));

    if (X <= Border || Z <= Border)
    {
        Out.Color = float4(1.0f, 0.0f, 0.0f, Alpha);
        return Out;
    }

    clip(-1);
    return Out;
}