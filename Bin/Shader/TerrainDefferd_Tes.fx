#include "GValue.fx"
#include "PixValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

cbuffer TerrainBuffer : register(b0)
{
    int FloorCount;
    int VtxX;
    int VtxZ;
    int TextureCount;
    int IsBump[4];
}

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

Vtx_Out VS_TerrainDefferd(Vtx_In _In)
{
    Vtx_Out Out = (Vtx_Out) 0.0f;

    Out.Pos.w = 1.0f;
    Out.Pos = mul(_In.Pos, World);
    Out.Uv = _In.Uv;
    Out.Color.rgba = _In.Color.rgba;
    
    return Out;
}

// Hull Shader

struct TessFactor
{
    float arrEdge[3] : SV_TessFactor;
    float InsideFactor : SV_InsideTessFactor;
};

float CalTessLv(float _MinDist, float _MaxDist, float _MaxTess, float _MinTess, float3 _WorldPos)
{
    float3 Dis = CameraPos.xyz - _WorldPos;
    //Dis.y = 0.0f;

    float Dist = length(Dis.xyz);
    float Ratio = (Dist - _MinDist) / (_MaxDist - _MinDist);

    return pow(2, saturate(floor(lerp(4, 0, Ratio)) / 4.0f) * 4.0f);

}

TessFactor HS_ConstantData(InputPatch<Vtx_Out, 3> _VtxData)
{
    TessFactor Out = (TessFactor) 0.0f;

    float3 StdPos = (_VtxData[0].Pos.xyz + _VtxData[1].Pos.xyz) / 2.0f;
    float XRatio = _VtxData[2].Pos.x - _VtxData[1].Pos.x;
    float ZRatio = _VtxData[2].Pos.z - _VtxData[0].Pos.z;

    float3 XPos = float3(StdPos.x + XRatio, StdPos.y, StdPos.z);
    float3 ZPos = float3(StdPos.x, StdPos.y, StdPos.z + ZRatio);

    float FTess = CalTessLv(50.0f, 500.0f, 0.0f, 0.0f, StdPos);
    float FXTess = CalTessLv(50.0f, 500.0f, 0.0f, 0.0f, XPos);
    float FZTess = CalTessLv(50.0f, 500.0f, 0.0f, 0.0f, ZPos);

    if (FTess < FZTess)
    {
        Out.arrEdge[0] = FZTess;
    }
    else
    {
        Out.arrEdge[0] = FTess;
    }

    if (FZTess < FXTess)
    {
        Out.arrEdge[1] = FXTess;
    }
    else
    {
        Out.arrEdge[1] = FTess;
    }

    Out.arrEdge[2] = FTess;
    Out.InsideFactor = FTess;

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
Hull_Out HS_TerrainDefferd(InputPatch<Vtx_Out, 3> _PatchData, uint i : SV_OutputControlPointID, uint _patchID : SV_PrimitiveID)
{
    Hull_Out Out = (Hull_Out) 0.0f;

    Out.Pos = _PatchData[i].Pos;
    Out.Uv = _PatchData[i].Uv;

    return Out;
}

// Hull Shader End

// Domain Shader

struct Domain_Out
{
    float4 Pos : SV_Position;
    float2 Uv : TEXCOORD;

    float4 WorldPos : POSITION;
    float4 ViewPos : POSITION1;
    float4 ProjPos : POSITION2;
};

[domain("tri")]
Domain_Out DS_TerrainDefferd(TessFactor _patchFactor, float3 _TriRatio : SV_DomainLocation, const OutputPatch<Hull_Out, 3> _ControlPoints)
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

// Domain Shader End

PS_DefferdOut PS_TerrainDefferd(Domain_Out _In)
{
    PS_DefferdOut Out = (PS_DefferdOut) 0.0f;
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    float3 Right = float3(_In.WorldPos.x + 1.0f, _In.WorldPos.y, _In.WorldPos.z);
    float3 Top = float3(_In.WorldPos.x, _In.WorldPos.y, _In.WorldPos.z + 1.0f);

    float3 BiNormal = normalize(Top - _In.WorldPos.xyz);
    float3 Tangent = normalize(Right - _In.WorldPos.xyz);
    float3 Normal = normalize(cross(BiNormal, Tangent));

    Color *= GetMultiTexColor(8, 8, _In.Uv, 0.0f);
    float4 BumpNormal = CalMultiBump(8, 8, _In.Uv, 1.0f, float4(Tangent, 0.0f), float4(BiNormal, 0.0f), float4(Normal, 0.0f));

    BumpNormal = mul(BumpNormal, View);

    float2 SpUv;

    SpUv.x = _In.Uv.x / VtxX;
    SpUv.y = _In.Uv.y / VtxZ;

   for (int i = 0; i < FloorCount; ++i)
   {
       float4 Ratio = GetTexColor(i, i, SpUv);
       float RatioValue = (Ratio.x + Ratio.y + Ratio.z) / 3.0f;
       float4 FloorColor = GetMultiTexColor(9 + i, 9 + i, _In.Uv, 0.0f);
       float4 SrcColor = Color;

       FloorColor.xyz *= RatioValue;
       SrcColor.xyz *= (1.0f - Ratio.x);
       Color = FloorColor + SrcColor;

       if (RatioValue >= 0.9f)
       {
           //_In.Normal = CalMultiBump(9 + i, 9 + i, _In.Uv, 1.0f, _In.Tangent, _In.BiNormal, _In.Normal);
       }
   }

    Out.Diffuse.rgb = Color.xyz;
    Out.Diffuse.a = 1.0f;
    Out.Normal = BumpNormal;
    Out.Normal.a = 1.0f;
    Out.Position = _In.ViewPos;
    Out.Depth.x = Out.Position.z;
    Out.Depth.w = 1.0f;

    return Out;
}