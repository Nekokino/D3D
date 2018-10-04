#include "PreCom.h"
#include "NTFontRenderer.h"
#include "ResourceSystem.h"
#include "NTFont.h"
#include "NTWinShortCut.h"

NTFontRenderer::NTFontRenderer() : Font(nullptr), Color(0xffffffff), Size(0.0f), Mode(RM_MAX)
{
}


NTFontRenderer::~NTFontRenderer()
{
}

bool NTFontRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	return true;
}

bool NTFontRenderer::Init(const wchar_t* _FontName, int _Order)
{
	Font = ResourceSystem<NTFont>::Find(_FontName);

	if (false == Init(_Order))
	{
		return false;
	}

	if (Font == nullptr)
	{
		tassert(true);
		return false;
	}

	return true;
}

void NTFontRenderer::Render(const NTMAT & _VP)
{
	if (Mode == RM_MAX)
	{
		return;
	}

	if (String.size() <= 0)
	{
		return;
	}

	if (Size <= 0.0f)
	{
		return;
	}
	
	SubTranformUpdate();

	wchar_t* Tmp = const_cast<wchar_t*>(String.c_str());

	NTRECT Rect;
	NTMAT SubWorldMat = GetSubWorldMat();
	Rect.Pos = NTVEC2(SubWorldMat.Matrix._41, SubWorldMat.Matrix._42);
	Rect.Size = NTVEC2(SubWorldMat.Matrix._11, SubWorldMat.Matrix._22);

	if (Mode == RM_NORMAL)
	{
		Rect.Pos.x += NTWinShortCut::GetMainWindow().GetWidthf() * 0.5f;
		Rect.Pos.y *= -1.0f;
		Rect.Pos.y += NTWinShortCut::GetMainWindow().GetHeightf() * 0.5f;
		Font->DrawFont(Tmp, Rect, Size, Color);
	}
	
	if (Mode == RM_RECT)
	{
		Font->DrawFontRect(Tmp, Rect, Size, Color);
	}

	NTWinShortCut::GetMainDevice().ResetContext();
}
