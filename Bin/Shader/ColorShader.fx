cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

struct VTXCOL_INPUT
{
	float4 Pos : POSITION;
	float4 Col : COLOR;
};

struct VTXCOL_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR;
};

struct PX_OUTPUT
{
	float4 Col : SV_Target;
};

cbuffer MulColor : register(b0)
{
	float4 CBColor;
}

VTXCOL_OUTPUT VS_Color(VTXCOL_INPUT _In)
{
	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.0f;

	OutData.Pos = mul(_In.Pos, WVP);
	OutData.Col = _In.Col;

	return OutData;
}

PX_OUTPUT PX_Color(VTXCOL_OUTPUT _In)
{
	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;

	OutData.Col = _In.Col * CBColor;

	return OutData;
}