#pragma once
#include "NTStCom.h"
#include  <list>
class NTLight : public NTStCom
{
public:
	friend class NTRenderSystem;

	enum LightType
	{
		Dir,
		Point,
		Spot,
	};

	class LightColor
	{
	public:
		NTVEC Diffuse; // Ȯ�걤
		NTVEC Specular; // �ݻ籤
		NTVEC Ambient; // ȯ�汤
	};

	class LightData
	{
	public:
		LightColor Color;
		NTVEC Pos;
		NTVEC Dir;
		float Range;
		float Angle;
		int Type;
	};

	class LightCBData
	{
	public:
		LightData ArrLight[10];
		int LightCount;
		int Dummy1;
		int Dummy2;
	};

public:
	int LightOrder;
	std::vector<int> RenderGroup;

private:
	LightData Data;

public:
	virtual bool Init();
	void MainUpdate() override;

private:
	template<typename ...Rest>
	void PushLayer(int _Data, Rest ...Arg)
	{
		RenderGroup.push_back(_Data);
		PushLayer(Arg...);
	}

	void PushLayer() {}

public:
	NTLight();
	~NTLight();
};

