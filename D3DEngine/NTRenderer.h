#pragma once
#include "NTMaterial.h"
#include "NTMesh.h"
#include "NTBlend.h"
#include "NTSubTransform.h"
#include "NTDevice.h"

class NTCamera;
class NTRenderer : public NTSubTransform
{
protected:
	friend class NTRenderSystem;

	Autoptr<NTMesh> Mesh;
	Autoptr<NTMaterial> Material;
	Autoptr<NTDevice::RasterState> RasterState;

	int Order;

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

public:
	NTRenderer();
	~NTRenderer();
};

