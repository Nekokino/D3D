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

VTXTEX_OUTPUT VS_Texture(VTXTEX_INPUT _In)
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

PX_OUTPUT PX_Texture(VTXTEX_OUTPUT _In)
{
	float4 Col = Tex_0.Sample(Samp_0, _In.Uv);

	if (0 >= Col.a)
	{
		clip(-1);
	}

	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;

	//OutData.Col = Col;
	OutData.Col = Col * CBColor;

	return OutData;
}