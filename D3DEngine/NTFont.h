#pragma once
#include "NTResource.h"
#include "DirectHeader.h"
#include "FW1CompileSettings.h"
#include "FW1FontWrapper.h"

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "FW1FontWrapperD32")
	#else
		#pragma comment(lib, "FW1FontWrapperD64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "FW1FontWrapperR32")
	#else
		#pragma comment(lib, "FW1FontWrapperR64")
	#endif
#endif


class NTFont : public NTResource
{
private:
	IFW1Factory* FW1Factory;
	IFW1FontWrapper* FW1FontWrapper;

public:
	bool Create(const wchar_t* _FontName);
	void DrawFont(wchar_t* _Str, NTVEC2 _Pos, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	void DrawFontRect(wchar_t* _Str, NTRECT _Rect, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
public:
	NTFont();
	~NTFont();
};

