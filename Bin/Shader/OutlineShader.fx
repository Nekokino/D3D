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

VTXTEX_OUTPUT VS_Outline(VTXTEX_INPUT _In)
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

cbuffer Outline : register(b2)
{
	float4 Line = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

cbuffer OutlineCol : register(b3)
{
	float4 OutCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

PX_OUTPUT PX_Outline(VTXTEX_OUTPUT _In)
{
	float2 UV = float2(Start.x + (Size.x * _In.Uv.x), Start.y + (Size.y * _In.Uv.y));

	float4 Col = Tex_0.Sample(Samp_0, UV);

	PX_OUTPUT OutData = (PX_OUTPUT)0.0f;

	float2 UpC = float2(UV.x, UV.y - Line.y * Line.z); // 아 이게 두께가 두꺼워지면 이지랄을 더해야하는데 이걸 어떻게 줄이지?
	float2 DownC = float2(UV.x, UV.y + Line.y* Line.z);
	float2 RightC = float2(UV.x + Line.x* Line.z, UV.y);
	float2 LeftC = float2(UV.x - Line.x* Line.z, UV.y);

	if (UpC.y <= 0.0f) {	UpC = UV;	}// 자르는 이미지도 쓰레기값 참조해서 이상생길수 있으니 일단 기억
	if (DownC.y >= 1.0f){	DownC = UV;	}
	if (RightC.x >= 1.0f){RightC = UV;	}
	if (LeftC.x <= 0.0f){LeftC = UV;	}

	float4 Up = Tex_0.Sample(Samp_0, UpC);
	float4 Down = Tex_0.Sample(Samp_0, DownC);
	float4 Right = Tex_0.Sample(Samp_0, RightC);
	float4 Left = Tex_0.Sample(Samp_0, LeftC);

	if (0 >= Col.a)
	{
		if (Up.a > 0.0f)
		{
			OutData.Col = OutCol;
			return OutData;
		}

		if (Down.a > 0.0f)
		{
			OutData.Col = OutCol;
			return OutData;
		}

		if (Right.a > 0.0f)
		{
			OutData.Col = OutCol;
			return OutData;
		}

		if (Left.a > 0.0f)
		{
			OutData.Col = OutCol;
			return OutData;
		}
		clip(-1);
	}

	OutData.Col = Col * CBColor;

	return OutData;
}