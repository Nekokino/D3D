#pragma once
#include "NTRenderer.h"
#include "NTImage.h"

class NTBlend;
class NTSpRenderer : public NTRenderer
{
protected:
	Autoptr<NTImage> Image;
	NTCOLOR Color;
	size_t ImgIndex;
public:
	void Render(const NTMAT& _VP) override;

	size_t GetImageIndex()
	{
		return ImgIndex;
	}

	void SetImageIndex(size_t _Index)
	{
		ImgIndex = _Index;
	}

	NTCOLOR GetColor()
	{
		return Color;
	}

	void SetColor(NTCOLOR _Color)
	{
		Color = _Color;
	}

	void SetImage(const wchar_t* _ImageName);
	Autoptr<NTImage> GetImage()
	{
		return Image;
	}
	void SetOriginScale();
	NTVEC GetOriginScale()
	{
		return Image->GetTex()->GetImageSize();
	}

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, int _Order = 0);

public:
	NTSpRenderer();
	~NTSpRenderer();
};

