#include "GValue.fx"
#include "PixValue.fx"
#include "ForwardLight.fx"
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
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};

Vtx_Out VS_TesTest(Vtx_In _In)
{
    Vtx_Out Out = (Vtx_Out) 0.0f;

    Out.Pos.w = 1.0f;
    Out.Pos = mul(_In.Pos, World);
    Out.Uv = _In.Uv;

    return Out;
}

struct TessFactor
{
    float arrEdge[3] : SV_TessFactor;
    float InsideFactor : SV_InsideTessFactor;
};

TessFactor HS_ConstantData(InputPatch<Vtx_Out, 3> _VtxData)
{
    TessFactor Out = (TessFactor) 0.0f;

    Out.arrEdge[0] = 3.0f;
    Out.arrEdge[1] = 3.0f;
    Out.arrEdge[2] = 3.0f;

    Out.InsideFactor = 3.0f;

    return Out;
}

struct Hull_Out
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

[domain("tri")]
[partitioning("integer")]
[outputcontrolpoints(3)]
[maxtessfactor(64.0f)]
[patchconstantfunc("HS_ConstantData")]
[outputtopology("triangle_cw")]
Hull_Out HS_TesTest(InputPatch<Vtx_Out, 3> _PatchData, uint i : SV_OutputControlPointID, uint _patchID : SV_PrimitiveID)
{
    Hull_Out Out = (Hull_Out) 0.0f;

    Out.Pos = _PatchData[i].Pos;
    Out.Uv = _PatchData[i].Uv;

    return Out;
}

struct Domain_Out
{
    float4 Pos : SV_Position;
    float2 Uv : TEXCOORD;

    float4 WorldPos : POSITION;
    float4 ViewPos : POSITION1;
    float4 ProjPos : POSITION2;
};

[domain("tri")]
Domain_Out DS_TesTest(TessFactor _patchFactor, float3 _TriRatio : SV_DomainLocation, const OutputPatch<Hull_Out, 3> _ControlPoints)
{
    Domain_Out Out = (Domain_Out) 0.0f;

    float3 WorldPos = (_TriRatio.x * _ControlPoints[0].Pos.xyz) + (_TriRatio.y * _ControlPoints[1].Pos.xyz) + (_TriRatio.z * _ControlPoints[2].Pos.xyz);

    Out.WorldPos.xyz = WorldPos;
    Out.WorldPos.w = 1.0f;
    Out.ViewPos = mul(Out.WorldPos, View);
    Out.Pos = mul(Out.ViewPos, Projection);
    Out.ProjPos = Out.Pos;
    Out.Uv = (_TriRatio.x * _ControlPoints[0].Uv) + (_TriRatio.y * _ControlPoints[1].Uv) + (_TriRatio.z * _ControlPoints[2].Uv);

   

    return Out;

}

struct Pix_Out
{
    float4 Color : SV_Target;
};

Pix_Out PS_TesTest(Domain_Out _In)
{
    Pix_Out Out = (Pix_Out) 0.0f;

    Out.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return Out;

}