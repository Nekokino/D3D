#pragma once
#include "NTMaterial.h"
#include "NTMesh.h"
#include "NTBlend.h"
#include "NTSubTransform.h"

class NTRenderer : public NTSubTransform
{
protected:
	Autoptr<NTMesh> Mesh;
	Autoptr<NTMaterial> Material;

	int Order;

public:
	bool SetMesh(const wchar_t* _Mesh);
	bool SetMaterial(const wchar_t* _Material);

	int GetOrder()
	{
		return Order;
	}

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(const NTMAT& _VP) = 0;

public:
	NTRenderer();
	~NTRenderer();
};

