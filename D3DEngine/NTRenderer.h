#pragma once
#include "NTMaterial.h"
#include "NTMesh.h"
#include "NTBlend.h"
#include "NTSubTransform.h"
#include "NTDevice.h"

class DrawData
{
public:
	friend NTRenderer;

public:
	UINT Mesh;
	UINT Vtx;
	UINT Sub;
	UINT Mat;

private:
	DrawData(UINT _Mesh, UINT _Vtx, UINT _Sub, UINT _Mat) : Mesh(_Mesh), Vtx(_Vtx), Sub(_Sub), Mat(_Mat)
	{

	}
};

enum NTDRAWMODE
{
	BASE,
	DATA,
};

class RenderOption
{
private:
	friend class NTRenderer;

public:
	int IsLight;
	int IsDefferdOrForward;
	int IsLightVtxOrPix;
	int TexCount;
	int IsBoneAni;
	NTDRAWMODE DrawMode;
	int Dummy2;
	int Dummy3;

private:
	TextureData ArrTex[8];

public:
	RenderOption() : IsLight(1), TexCount(0), IsLightVtxOrPix(1), IsDefferdOrForward(1), IsBoneAni(0), DrawMode(NTDRAWMODE::BASE)
	{

	}
};

class NTCamera;
class NTRenderer : public NTSubTransform
{
protected:
	friend class NTRenderSystem;

	MatrixData MatData;
	Autoptr<NTDevice::RasterState> RasterState;

	std::vector<Autoptr<NTMesh>> MeshVec;
	std::vector<Autoptr<NTMaterial>> MaterialVec;
	std::vector<DrawData> DrawDataVec;

	int Light;
	int Order;

public:
	RenderOption RndOpt;

public:
	size_t GetMeshCount() { return MeshVec.size(); }
	size_t GetMaterialCount() { return MaterialVec.size(); }

	void AddDrawData(UINT _Mesh, UINT _Vtx, UINT _Sub, UINT _Mat);

public:
	bool SetMesh(const wchar_t* _Mesh, size_t _Index = 0);
	bool SetMaterial(const wchar_t* _Material, size_t _Index = 0);
	bool SetMesh(Autoptr<NTMesh> _Mesh, size_t _Index = 0);
	void SetRasterState(const wchar_t* _Name);

	int GetOrder()
	{
		return Order;
	}

	Autoptr<NTMaterial> GetMaterial(int _Index = 0);

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(Autoptr<NTCamera> _Camera) = 0;
	virtual void MaterialConstBufferUpdate(int _Index = 0);

private:
	void RenderUpdate();
	void RenderAfterUpdate();

protected:
	virtual void TransformUpdate(Autoptr<NTCamera> _Cam);
	virtual void TransformConstBufferUpdate();
	virtual void MeshUpdate(int _Index = 0);
	virtual void MaterialUpdate(int _Index = 0);
	virtual void MaterialTextureNSamplerUpdate(int _Index = 0);
	virtual void TargetMeshUpdate(UINT _Mesh, UINT _Vtx, UINT _Idx);

public:
	NTRenderer();
	~NTRenderer();
};