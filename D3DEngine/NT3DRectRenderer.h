#pragma once
#include "NTRenderer.h"

class NT3DRectRenderer : public NTRenderer
{
public:
	MatrixData MatData;

public:
	void Render(const NTMAT& _VP) override;

public:
	bool Init(int _Order = 0) override;
public:
	NT3DRectRenderer();
	~NT3DRectRenderer();
};

