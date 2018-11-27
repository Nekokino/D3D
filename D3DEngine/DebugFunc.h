#pragma once
#include "DirectHeader.h"
#include "FW1FontWrapper.h"
#include "NTMesh.h"
#include <list>
#include <string>

class DebugFunc
{
private:
	friend class NTWindow;

private:
	class DbgLog
	{
	public:
		std::wstring Msg;
		NTVEC Color;
	};

	static bool bIsDebug;
	static float LogSize;
	static std::list<DbgLog>::iterator ListStartIter;
	static std::list<DbgLog>::iterator ListEndIter;
	static std::list<DbgLog> Log;

public:
	static bool IsDebug()
	{
		return bIsDebug;
	}

public:
	static void Init();
	static void DrawRect(const NTRECT& _Rect, float _Border = 1);
	static void DrawCircle(const NTCIRCLE& _Circle, float _Border = 1);
	static void DrawFont(wchar_t* _Str, NTVEC2 _Pos, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	static void DrawLog(wchar_t* _Str, NTVEC Color = NTVEC::ONE);

	static void DrawDbgMesh(Autoptr<NTMesh> _Mesh);

public:
	static void RenderLog();
	static void TargetDebug();

public:
	DebugFunc();
	~DebugFunc();
};

