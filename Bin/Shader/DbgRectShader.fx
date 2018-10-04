cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

Texture2D Tex_0 : register(t0);
SamplerState Samp_0 : register(s0);

struct VTXCOL_INPUT
{
	float4 Pos : POSITION;
	float2 Uv : TEXCOORD;
};

struct VTXCOL_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Uv : TEXCOORD;
};

struct PX_OUTPUT
{
	float4 Col : SV_Target;
};

VTXCOL_OUTPUT VS_DbgRect(VTXCOL_INPUT _In)
{
	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.0f;

	OutData.Pos = mul(_In.Pos, WVP);
	OutData.Uv = _In.Uv;

	return OutData;
}

cbuffer Outline : register(b2)
{
	float4 Line = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

PX_OUTPUT PX_DbgRect(VTXCOL_OUTPUT _In)
{
	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;

	if (_In.Uv.x > Line.x && _In.Uv.x < 1.0f - Line.x && _In.Uv.y > Line.y && _In.Uv.y < 1.0f - Line.y)
	{
		return OutData;
	}
	OutData.Col = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return OutData;
}