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
    float4 InvDir;
    float Range;
    float Angle;
    int Type;
    int Dummy;
};

LightColor CalDirLight(float4 _ViewPos, float4 _ViewNormal, LightData _LightInfo)
{
    LightColor CalColor = (LightColor) 0.0f;

    float4 Light = (float4)  0.0f;
    float Factor = 1.0f;

    if (_LightInfo.Type == 0)
    {
        Light = _LightInfo.InvDir;
    }
    else if (_LightInfo.Type == 1)
    {
        Light = normalize(_LightInfo.Pos - _ViewPos);
        float Dis = distance(_LightInfo.Pos, _ViewPos);

        Factor = saturate((_LightInfo.Range - Dis) / _LightInfo.Range);
    }
    else if (_LightInfo.Type == 2)
    {

    }

    Light = normalize(Light);
    CalColor.Diffuse = _LightInfo.Color.Diffuse * saturate(dot(_ViewNormal, Light)) * Factor;
    

    float3 Reflect = normalize(2.0f * dot(Light, _ViewNormal) * _ViewNormal - Light);
    float3 Eye = normalize(-_ViewPos);

    CalColor.Specular = _LightInfo.Color.Specular * pow(saturate(dot(Eye, Reflect)), 1) * Factor; 
    CalColor.Ambient = _LightInfo.Color.Ambient;

    return CalColor;
}