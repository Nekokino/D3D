#pragma once
#include "NTRenderer.h"
#include "NTImage.h"

class NT3DMeshRenderer : public NTRenderer
{
public:
	MatrixData MatData;
	Autoptr<NTImage> Image;

public:
	void Render(Autoptr<NTCamera> _Camera) override;

public:
	void SetImage(const wchar_t* _Name);

public:
	bool Init(int _Order = 0) override;
public:
	NT3DMeshRenderer();
	~NT3DMeshRenderer();
};

