#include "PreCom.h"
#include "NTFont.h"
#include "NTWinShortCut.h"

NTFont::NTFont() : FW1Factory(nullptr), FW1FontWrapper(nullptr)
{
}


NTFont::~NTFont()
{
	if (nullptr != FW1Factory)
	{
		FW1Factory->Release();
	}

	if (nullptr != FW1FontWrapper)
	{
		FW1FontWrapper->Release();
	}
}

bool NTFont::Create(const wchar_t * _FontName)
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &FW1Factory))
	{
		tassert(true);
		return false;
	}

	if (S_OK != FW1Factory->CreateFontWrapper(NTWinShortCut::GetDevice(), _FontName, &FW1FontWrapper))
	{
		tassert(true);
		return false;
	}

	return true;
}

void NTFont::DrawFont(wchar_t * _Str, NTVEC2 _Pos, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag)
{
	FW1FontWrapper->DrawString(NTWinShortCut::GetContext(), _Str, _Size, _Pos.x, _Pos.y, _Color, _Flag);
}

void NTFont::DrawFontRect(wchar_t* _Str, NTRECT _Rect, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag)
{
	FW1_RECTF RangeRect;

	RangeRect.Top = _Rect.Bottom();
	RangeRect.Bottom = _Rect.Top();
	RangeRect.Left = _Rect.Left();
	RangeRect.Right = _Rect.Right();

	FW1FontWrapper->DrawString(NTWinShortCut::GetContext(), _Str, nullptr, _Size, &RangeRect, _Color, nullptr, nullptr, _Flag);
}