
struct PS_DefferdOut
{
    float4 Diffuse : SV_Target;
    float4 Position : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
};

struct PS_ForwardOut
{
    float4 Diffuse : SV_Target;
};

struct PS_MergeOut
{
    float4 MergeColor : SV_Target;
};

struct VS_DefferdLightIn
{
    float4 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

struct VS_DefferdLightOut
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

struct PS_DefferdLightOut
{
    float4 Diffuse : SV_Target;
    float4 Specular : SV_Target1;
};