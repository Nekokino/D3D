#pragma once
#include "NTStCom.h"
#include <list>
#include <set>

class NTCamera;
class NTMesh;
class NTMaterial;
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
		NTVEC InvDir;
		float Range;
		float Angle;
		int Type;
		int Dummy;
	};

	class LightCBData
	{
	public:
		LightData ArrLight[10];
		int LightCount;
		int Dummy1;
		int Dummy2;
		int Dummy3;
	};

private:
	LightType Type;
	LightData Data;

	std::set<int> RenderGroup;

public:
	MatrixData MatData;
	Autoptr<NTMesh> LightMesh;
	Autoptr<NTMaterial> LightMat;
	Autoptr<NTMesh> VolumeMesh;
	Autoptr<NTMaterial> VolumeMaterial;

public:
	virtual bool Init();
	void EndUpdate() override;

public:
	bool IsLight(int _Group) const
	{
		std::set<int>::iterator FindIter = RenderGroup.find(_Group);

		if (FindIter == RenderGroup.end())
		{
			return false;
		}

		return true;
	}

public:
	template<typename ...Rest>
	void PushLightLayer(Rest ...Arg)
	{
		PushLayer(Arg...);
	}

public:
	template<typename ...Rest>
	void PushLayer(int _Data, Rest... Arg)
	{
		std::set<int>::iterator FindIter = RenderGroup.find(_Data);

		if (FindIter == RenderGroup.end())
		{
			RenderGroup.insert(_Data);
		}

		PushLayer(Arg...);
	}

	void PushLayer() {};

	void SetLightType(LightType _Type);

private:
	void LightRender(Autoptr<NTCamera> _Cam);
	void CalLightData(Autoptr<NTCamera> _Cam);
public:
	NTLight();
	~NTLight();
};

