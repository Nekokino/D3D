cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WVP;
}

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
   
    Out.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}