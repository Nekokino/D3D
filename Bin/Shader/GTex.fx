Texture2D Tex_0 : register(t0);
Texture2D Tex_1 : register(t1);
Texture2D Tex_2 : register(t2);
Texture2D Tex_3 : register(t3);
Texture2D Tex_4 : register(t4);
Texture2D Tex_5 : register(t5);
Texture2D Tex_6 : register(t6);
Texture2D Tex_7 : register(t7);

SamplerState Sam_0 : register(s0);
SamplerState Sam_1 : register(s1);
SamplerState Sam_2 : register(s2);
SamplerState Sam_3 : register(s3);
SamplerState Sam_4 : register(s4);
SamplerState Sam_5 : register(s5);
SamplerState Sam_6 : register(s6);
SamplerState Sam_7 : register(s7);

float4 GetColor(Texture2D _Tex, int _Samp, float2 _Uv)
{
    switch (_Samp)
    {
        case 0:
            return _Tex.Sample(Sam_0, _Uv);
        case 1:
            return _Tex.Sample(Sam_1, _Uv);
        case 2:
            return _Tex.Sample(Sam_2, _Uv);
        case 3:
            return _Tex.Sample(Sam_3, _Uv);
        case 4:
            return _Tex.Sample(Sam_4, _Uv);
        case 5:
            return _Tex.Sample(Sam_5, _Uv);
        case 6:
            return _Tex.Sample(Sam_6, _Uv);
        case 7:
            return _Tex.Sample(Sam_7, _Uv);
        default:
            return float4(0.0f);

    }
}

float4 GetTexColor(int _Tex, int _Samp, float2 _Uv)
{
    switch (_Tex)
    {
        case 0:
            return GetColor(Tex_0, _Samp, _Uv);
        case 1:
            return GetColor(Tex_1, _Samp, _Uv);
        case 2:
            return GetColor(Tex_2, _Samp, _Uv);
        case 3:
            return GetColor(Tex_3, _Samp, _Uv);
        case 4:
            return GetColor(Tex_4, _Samp, _Uv);
        case 5:
            return GetColor(Tex_5, _Samp, _Uv);
        case 6:
            return GetColor(Tex_6, _Samp, _Uv);
        case 7:
            return GetColor(Tex_7, _Samp, _Uv);
        default:
            return float4(0.0f);

    }

}