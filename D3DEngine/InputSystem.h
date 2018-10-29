#pragma once
#include <vector>
#include <unordered_map>

#include "StlAssist.h"
#include "RefNAutoptr.h"

enum KEY_ERROR
{
	KEY_OK,
	KEY_CREATE_ERROR,
	KEY_CREATE_FIND_ERROR,
	KEY_MAX,
};

class WinCore;
class InputSystem
{
public:
	friend WinCore;

public:
	static const char ERROR_MSG[KEY_MAX][256];

private:
	class KeyData : public RefCounter
	{
	private:
		static const char BinaryIdle;
		static const char BinaryUp;
		static const char BinaryDown;
		static const char BinaryPressed;
		static const char RBinaryIdle;
		static const char RBinaryUp;
		static const char RBinaryDown;
		static const char RBinaryPressed;

	public:
		friend InputSystem;

	private:
		std::vector<int> KeyDataList;
		char Data;
		float PushTime;

	public:
		template<typename T, typename ... Rest>
		void InsertKeyData(const T& _Key, Rest... _Arg)
		{
			KeyDataList.push_back(_Key);
			InsertKeyData(_Arg...);
		}

		void InsertKeyData()
		{

		}

	private:
		bool AllKeyCheck();
		void Update();

		bool IsUp();
		bool IsIdle();
		bool IsDown();
		bool IsPressed();
		bool IsOver(float _Time);
		bool IsOverRepeat(float _Time);

	public:
		KeyData(const size_t _Size);
		~KeyData();
	};

public:
	static const char* ErrorMsg(KEY_ERROR _Msg);

private:
	static std::unordered_map<std::wstring, Autoptr<KeyData>> KeyMap;
	static std::unordered_map<std::wstring, Autoptr<KeyData>>::iterator MapStartIter;
	static std::unordered_map<std::wstring, Autoptr<KeyData>>::iterator MapEndIter;

public:
	template<typename ... Rest>
	static KEY_ERROR CreateKey(const wchar_t* _Name, Rest ... _Arg)
	{
		Autoptr<KeyData> Key = MapFind<Autoptr<KeyData>>(KeyMap, _Name);

		if (nullptr != Key)
		{
			return KEY_CREATE_FIND_ERROR;
		}

		KeyData* NewKeyData = new KeyData(sizeof...(_Arg));
		NewKeyData->InsertKeyData(_Arg...);
		KeyMap.insert(std::unordered_map<std::wstring, Autoptr<KeyData>>::value_type(_Name, NewKeyData));
		return KEY_OK;
	}

public:
	static bool IsUp(const wchar_t* _Name);
	static bool IsIdle(const wchar_t* _Name);
	static bool IsDown(const wchar_t* _Name);
	static bool IsPressed(const wchar_t* _Name);
	static bool IsOver(const wchar_t* _Name, float _Time);
	static bool IsOverRepeat(const wchar_t* _Name, float _Time);
	static bool IsKey(const wchar_t* _Name);

private:
	static void Update();

private:
	static POINT Point;
	static NTVEC2 MousePos;
	static NTVEC2 LastMousePos;
	static NTVEC2 MouseDir;

public:
	static NTVEC2 GetMousePos()
	{
		return MousePos;
	}

	static NTVEC2 GetMouseDir()
	{
		return MouseDir;
	}
public:
	InputSystem();
	~InputSystem();
};

