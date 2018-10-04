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

VTXTEX_OUTPUT VS_Img(VTXTEX_INPUT _In)
{
	VTXTEX_OUTPUT OutData = (VTXTEX_OUTPUT)0.0f;

	OutData.Pos = mul(_In.Pos, WVP);
	OutData.Uv = _In.Uv;

	return OutData;
}

cbuffer MulColor : register(b0)
{
	float4 CBColor;
}

cbuffer ImageUv : register(b1)
{
	float2 Start = float2(0.0f, 0.0f);
	float2 Size = float2(1.0f, 1.0f);
};

PX_OUTPUT PX_Img(VTXTEX_OUTPUT _In)
{
	float4 Col = Tex_0.Sample(Samp_0, float2(Start.x + (Size.x * _In.Uv.x), Start.y + (Size.y * _In.Uv.y)));

	if (0 >= Col.a)
	{
		clip(-1);
	}

	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;

	OutData.Col = Col * CBColor;

	return OutData;
}