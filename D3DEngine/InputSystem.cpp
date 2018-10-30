#include "PreCom.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include <Windows.h>
#include "NTWinShortCut.h"

//////////////////////////////////////////////////////// Àü¿ª

std::unordered_map<std::wstring, Autoptr<InputSystem::KeyData>> InputSystem::KeyMap;
std::unordered_map<std::wstring, Autoptr<InputSystem::KeyData>>::iterator InputSystem::MapStartIter;
std::unordered_map<std::wstring, Autoptr<InputSystem::KeyData>>::iterator InputSystem::MapEndIter;

const char InputSystem::KeyData::BinaryUp		= (char)0b00000001;
const char InputSystem::KeyData::BinaryIdle		= (char)0b00000010;
const char InputSystem::KeyData::BinaryDown		= (char)0b00000100;
const char InputSystem::KeyData::BinaryPressed	= (char)0b00001000;
const char InputSystem::KeyData::RBinaryUp		= (char)0b11111110;
const char InputSystem::KeyData::RBinaryIdle	= (char)0b11111101;
const char InputSystem::KeyData::RBinaryDown	= (char)0b11111011;
const char InputSystem::KeyData::RBinaryPressed = (char)0b11110111;

POINT InputSystem::Point;
NTVEC2 InputSystem::MousePos;
NTVEC2 InputSystem::LastMousePos;
NTVEC2 InputSystem::MouseDir;

const char InputSystem::ERROR_MSG[KEY_MAX][256] =
{
	{"OK"},
	{"Create Error : Overlapped Name"},
	{"Set Error Message Here"}
};

const char* InputSystem::ErrorMsg(KEY_ERROR _Msg)
{
	return ERROR_MSG[_Msg];
}

//////////////////////////////////////////////////////////// KeyData °´Ã¼

InputSystem::KeyData::KeyData(const size_t _Size)
{
	KeyDataList.reserve(_Size);
}

InputSystem::KeyData::~KeyData()
{

}

bool InputSystem::KeyData::AllKeyCheck()
{
	for (size_t i = 0; i < KeyDataList.size(); i++)
	{
		if (0 == GetAsyncKeyState(KeyDataList[i]))
		{
			return false;
		}
	}

	return true;
}

void InputSystem::KeyData::Update()
{
	bool Check = AllKeyCheck();

	if (true == Check)
	{
		PushTime = DTime;

		if (0 != (Data & BinaryIdle))
		{
			Data &= RBinaryUp;
			Data &= RBinaryIdle;
			Data |= BinaryDown;
			Data |= BinaryPressed;
		}
		else if (0 == (Data & BinaryIdle))
		{
			Data &= RBinaryUp;
			Data &= RBinaryIdle;
			Data &= RBinaryDown;
			Data |= BinaryPressed;
		}
	}
	else
	{
		if (0 != (Data & BinaryPressed))
		{
			PushTime = 0.0f;

			Data |= BinaryUp;
			Data |= BinaryIdle;
			Data &= RBinaryDown;
			Data &= RBinaryPressed;
		}
		else if (0 == (Data & BinaryPressed))
		{
			Data &= RBinaryUp;
			Data |= BinaryIdle;
			Data &= RBinaryDown;
			Data &= RBinaryPressed;
		}
	}
}

bool InputSystem::KeyData::IsUp()
{
	return 0 != (Data & BinaryUp);
}

bool InputSystem::KeyData::IsIdle()
{
	return 0 != (Data & BinaryIdle);
}

bool InputSystem::KeyData::IsDown()
{
	return 0 != (Data & BinaryDown);
}

bool InputSystem::KeyData::IsPressed()
{
	return 0 != (Data & BinaryPressed);
}

bool InputSystem::KeyData::IsOver(float _Time)
{
	return PushTime >= _Time;
}

bool InputSystem::KeyData::IsOverRepeat(float _Time)
{
	bool Check = PushTime >= _Time;
	if (Check == true)
	{
		PushTime = 0.0f;
	}
	return Check;
}

///////////////////////////////////////////////////////////////  InputSystem

InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
}


void InputSystem::Update()
{
	GetCursorPos(&Point);
	ScreenToClient(NTWinShortCut::GetMainWindow().GetHWND(), &Point);

	LastMousePos = MousePos;
	MousePos.Vec.x = (float)Point.x;
	MousePos.Vec.y = (float)Point.y;

	MouseDir = MousePos - LastMousePos;

	MapStartIter = KeyMap.begin();
	MapEndIter = KeyMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		if (nullptr == MapStartIter->second)
		{
			continue;
		}

		MapStartIter->second->Update();
	}
}

bool InputSystem::IsUp(const wchar_t* _Name)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsUp();
}

bool InputSystem::IsIdle(const wchar_t* _Name)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsIdle();
}

bool InputSystem::IsDown(const wchar_t* _Name)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsDown();
}

bool InputSystem::IsPressed(const wchar_t* _Name)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsPressed();
}

bool InputSystem::IsOver(const wchar_t* _Name, float _Time)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsOver(_Time);
}

bool InputSystem::IsOverRepeat(const wchar_t* _Name, float _Time)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);
	if (nullptr == Key)
	{
		return false;
	}

	return Key->IsOverRepeat(_Time);
}

bool InputSystem::IsKey(const wchar_t * _Name)
{
	Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);

	if (nullptr == Key)
	{
		return false;
	}

	return true;
}
