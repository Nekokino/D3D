#pragma once
#include "NTMaterial.h"
#include "NTMesh.h"
#include "NTBlend.h"
#include "NTSubTransform.h"
#include "NTDevice.h"

class RenderOption
{
private:
	friend class NTRenderer;

public:
	int IsLight;
	int IsDefferdOrForward;
	int IsLightVtxOrPix;
	int TexCount;

private:
	TextureData ArrTex[12];

public:
	RenderOption() : IsLight(1), TexCount(0), IsLightVtxOrPix(1), IsDefferdOrForward(0)
	{

	}
};

class NTCamera;
class NTRenderer : public NTSubTransform
{
protected:
	friend class NTRenderSystem;

	MatrixData MatData;
	Autoptr<NTMesh> Mesh;
	Autoptr<NTMaterial> Material;
	Autoptr<NTDevice::RasterState> RasterState;
	
	int Light;
	int Order;

public:
	RenderOption RndOpt;

public:
	bool SetMesh(const wchar_t* _Mesh);
	bool SetMaterial(const wchar_t* _Material);
	void SetRasterState(const wchar_t* _Name);

	int GetOrder()
	{
		return Order;
	}

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(Autoptr<NTCamera> _Camera) = 0;

private:
	void RenderUpdate();
	void RenderAfterUpdate();

protected:
	virtual void TransformUpdate(Autoptr<NTCamera> _Cam);
	virtual void TransformConstBufferUpdate();
	virtual void MeshToMatUpdate();

public:
	NTRenderer();
	~NTRenderer();
};

