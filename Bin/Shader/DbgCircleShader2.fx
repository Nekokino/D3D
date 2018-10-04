cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

Texture2D Tex_0 : register(t0);
SamplerState Samp_0 : register(s0);

struct VTXTEX_INPUT
{
	float4 Pos : POSITION;
	float2 Uv : TEXCOORD;
};

struct VTXTEX_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Uv : TEXCOORD;
};

struct PX_OUTPUT
{
	float4 Col : SV_Target;
};

VTXTEX_OUTPUT VS_DbgCircle2(VTXTEX_INPUT _In)
{
	VTXTEX_OUTPUT OutData = (VTXTEX_OUTPUT)0.0f;

	OutData.Pos = mul(_In.Pos, WVP);
	OutData.Uv = _In.Uv;

	return OutData;
}

cbuffer Outline : register(b2)
{
	float4 Line = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

PX_OUTPUT PX_DbgCircle2(VTXTEX_OUTPUT _In)
{
	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;
	float2 Point = float2(0.5f, 0.5f);
	float2 Point2 = Point - _In.Uv;
	float tmp = sqrt(Point2.x * Point2.x + Point2.y * Point2.y);
	if (tmp < 0.5f - Line.x) // 크기에 비례해서 조절해야하나 싶음
	{
		return OutData;
	}
	float4 Col = Tex_0.Sample(Samp_0, _In.Uv);

	if (0 >= Col.a)
	{
		clip(-1);
	}

	Col.b = 1.0f;

	

	OutData.Col = Col;

	return OutData;
}