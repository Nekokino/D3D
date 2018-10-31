#pragma once
#include "NTRenderer.h"
class NT3DGrid : public NTRenderer
{
public:
	MatrixData MatData;

	float CurBlockSize;
	float InterBlockSize;

	NTVEC GridData;

public:
	void Render(Autoptr<NTCamera> _Camera) override;
	void DbgRender() override;

public:
	bool Init(int _Order = 0) override;

private:
	void CalData(Autoptr<NTCamera> _Camera);
public:
	NT3DGrid();
	~NT3DGrid();
};

