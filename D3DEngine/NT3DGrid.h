#pragma once
#include "NTRenderer.h"
class NT3DGrid : public NTRenderer
{
public:
	MatrixData MatData;

public:
	void Render(Autoptr<NTCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;
public:
	NT3DGrid();
	~NT3DGrid();
};

