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
    int Dummy;
};

cbuffer LightBuffer : register(b12)
{
    LightData Arr[10];
    int LightCount;
    int Dummy1;
    int Dummy2;
    int Dummy3;
}

LightColor CalDirLight(float4 _ViewPos, float4 _ViewNormal, LightData _LightInfo)
{
    LightColor CalColor = (LightColor) 0.0f;

    float4 Light = _LightInfo.Dir;

    CalColor.Diffuse = _LightInfo.Color.Diffuse * saturate(dot(_ViewNormal, normalize(Light)));
    Light = normalize(Light);

    float4 Reflect = normalize(2.0f * dot(Light, _ViewNormal) * _ViewNormal - Light);
    float4 Eye = -normalize(_ViewPos);

    CalColor.Specular = _LightInfo.Color.Specular * pow(saturate(dot(Eye, Reflect)), 10);
    CalColor.Ambient = _LightInfo.Color.Ambient;

    return CalColor;
}