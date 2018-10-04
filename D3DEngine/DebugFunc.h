#pragma once
#include "DirectHeader.h"
#include "FW1FontWrapper.h"

class DebugFunc
{
public:
	static void Init();
	static void DrawRect(const NTRECT& _Rect, float _Border = 1);
	static void DrawCircle(const NTCIRCLE& _Circle, float _Border = 1);
	static void DrawFont(wchar_t* _Str, NTVEC2 _Pos, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
public:
	DebugFunc();
	~DebugFunc();
};

