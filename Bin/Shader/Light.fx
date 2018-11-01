class LightColor
{
    float4 Diffuse;
    float4 Specular;
    float4 Ambient;
};

class LightData
{
    LightColor Color;
    float4 Pos;
    float4 Dir;
    float Range;
    float Angle;
    int Type;
};

cbuffer LightBuffer : register(b10)
{
    LightData Arr[10];
    int LightCount;
    int Dummy1;
    int Dummy2;
}