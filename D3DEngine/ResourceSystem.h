#pragma once

#include "RefNAutoptr.h"
#include "StlAssist.h"
#include <vector>
#include <unordered_map>
#include <string>

class WinCore;
class PathSystem
{
public:
	friend WinCore;

private:
	static std::wstring Root;
	static std::unordered_map<std::wstring, std::wstring> PathMap;
	

public:
	static void CreatePath(const wchar_t* _Key, const wchar_t* _FolderName);
	static void CreatePath(const wchar_t* _Key);
	static std::wstring CreateFilePath(const wchar_t* _Folder, const wchar_t* _FileName);
	static const wchar_t* FindPath(const wchar_t* _Key);
	static bool IsFile(const wchar_t* _Path);
	static void CreateAllPath(const wchar_t* _RootPath);
	static void CreateAllPathChild(const wchar_t* _RootPath, const wchar_t* _UpperPath, const wchar_t* _Path);

private:
	static void Init();

public:
	static const wchar_t* GetRoot() { return Root.c_str(); }

private:
	PathSystem() {}
	~PathSystem() {}
};

template<typename Res>
class ResourceSystem
{
private:
	static std::unordered_map<std::wstring, Autoptr<Res>> ResMap;
	static std::unordered_map<std::wstring, std::vector<Autoptr<Res>>> MultiResMap;

public:
	static Autoptr<Res> Find(const wchar_t* _Name)
	{
		return MapFind<Autoptr<Res>>(ResMap, _Name);
	}

	static Autoptr<Res> Create(const wchar_t* _Name)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create())
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4, typename V5>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static Autoptr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		Res* NewRes = new Res();
		NewRes->SetType();
		NewRes->SetName(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	static void LoadFromFolder(const wchar_t* _Path)
	{
		std::wstring FindPath = PathSystem::FindPath(_Path);
		std::wstring SearchOp = L"";

		SearchOp = FindPath + L"*.*";

		WIN32_FIND_DATA FindData;
		HANDLE Finder;

		Finder = FindFirstFileW(SearchOp.c_str(), &FindData);

		if (Finder == INVALID_HANDLE_VALUE)
		{
			return;
		}

		while (FindNextFileW(Finder, &FindData))
		{
			if (FindData.dwFileAttributes == 16) //디렉토리인 경우
			{
				if (wcscmp(FindData.cFileName, L"..") == 0) // 상위 디렉토리 한번 찍어줌
				{
					continue;
				}
				else
				{
					std::wstring tmp = _Path;
					tmp += L"\\";
					tmp += FindData.cFileName;

					if (nullptr == PathSystem::FindPath(FindData.cFileName))
					{
						PathSystem::CreatePath(FindData.cFileName, tmp.c_str());
					}
					LoadFromFolder(FindData.cFileName);
				}
			}
			else
			{
				std::wstring tmp = FindPath + FindData.cFileName;
				Res* NewRes = new Res();
				NewRes->SetName(FindData.cFileName);
				NewRes->SetPathkey(_Path);
				NewRes->SetPath(tmp.c_str());

				if (false == NewRes->Load())
				{
					delete NewRes;
					return;
				}

				ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(FindData.cFileName, NewRes));
			}
		}

		FindClose(Finder);
	}

	static void MultiLoadFromFolder(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::vector<Autoptr<Res>> NewVec;
		std::wstring FindPath = PathSystem::FindPath(_Path);
		std::wstring SearchOp = L"";

		SearchOp = FindPath + L"*.*";

		WIN32_FIND_DATA FindData;
		HANDLE Finder;

		Finder = FindFirstFileW(SearchOp.c_str(), &FindData);

		if (Finder == INVALID_HANDLE_VALUE)
		{
			return;
		}

		while (FindNextFileW(Finder, &FindData))
		{
			if (FindData.dwFileAttributes == 16) //디렉토리인 경우
			{
				if (wcscmp(FindData.cFileName, L"..") == 0) // 상위 디렉토리 한번 찍어줌
				{
					continue;
				}
				else
				{
					std::wstring tmp = _Path;
					tmp += L"\\";
					tmp += FindData.cFileName;
					
					if (nullptr == PathSystem::FindPath(FindData.cFileName))
					{
						PathSystem::CreatePath(FindData.cFileName, tmp.c_str());
					}
					MultiInsert(FindData.cFileName, NewVec);
				}
			}
			else
			{
				std::wstring tmp = FindPath + FindData.cFileName;
				Res* NewRes = new Res();
				NewRes->SetName(FindData.cFileName);
				NewRes->SetPathkey(_Path);
				NewRes->SetPath(tmp.c_str());

				if (false == NewRes->Load())
				{
					delete NewRes;
					return;
				}

				NewVec.push_back(NewRes);
			}
		}

		MultiResMap.insert(std::unordered_map<std::wstring, std::vector<Autoptr<Res>>>::value_type(_Name, NewVec));

		FindClose(Finder);
	}

	template<typename VECTOR>
	static void MultiInsert(const wchar_t* _Path, VECTOR& _Vector)
	{
		std::wstring FindPath = PathSystem::FindPath(_Path);
		std::wstring SearchOp = L"";

		SearchOp = FindPath + L"*.*";

		WIN32_FIND_DATA FindData;
		HANDLE Finder;

		Finder = FindFirstFileW(SearchOp.c_str(), &FindData);

		if (Finder == INVALID_HANDLE_VALUE)
		{
			return;
		}

		while (FindNextFileW(Finder, &FindData))
		{
			if (FindData.dwFileAttributes == 16) //디렉토리인 경우
			{
				if (wcscmp(FindData.cFileName, L"..") == 0) // 상위 디렉토리 한번 찍어줌
				{
					continue;
				}
				else
				{
					std::wstring tmp = _Path;
					tmp += L"\\";
					tmp += FindData.cFileName;

					PathSystem::CreatePath(FindData.cFileName, tmp.c_str());
					MultiInsert(FindData.cFileName, _Vector);
				}
			}
			else
			{
				std::wstring tmp = FindPath + FindData.cFileName;
				Res* NewRes = new Res();
				NewRes->SetName(FindData.cFileName);
				NewRes->SetPathkey(_Path);
				NewRes->SetPath(tmp.c_str());

				if (false == NewRes->Load())
				{
					delete NewRes;
					return;
				}

				_Vector.push_back(NewRes);
			}
		}

		FindClose(Finder);
	}

	static Autoptr<Res> Load(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::wstring FindPath = PathSystem::FindPath(_Path);
		FindPath += _Name;

		Res* NewRes = new Res();
		NewRes->SetName(_Name);
		NewRes->SetPathkey(_Path);
		NewRes->SetPath(FindPath.c_str());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	template<typename V1, typename V2>
	static Autoptr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1, V2 _2)
	{
		std::wstring FindPath = PathSystem::FindPath(_Path);

		FindPath += _Name;

		Res* NewRes = new Res();
		NewRes->SetName(_Name);
		NewRes->SetPathkey(_Path);
		NewRes->SetPath(FindPath.c_str());
		if (false == NewRes->Load(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	static Autoptr<Res> LoadFromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName)
	{
		std::wstring Path = PathSystem::FindPath(_PathKey);
		Path += _FileName;

		Res* NewRes = new Res();
		NewRes->SetName(_Key);
		NewRes->SetType();
		NewRes->SetPathkey(_PathKey);
		NewRes->SetPath(Path.c_str());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}

	template<typename V1>
	static Autoptr<Res> LoadFromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1)
	{
		std::wstring Path = PathSystem::FindPath(_PathKey);
		Path += _FileName;

		Res* NewRes = new Res();
		NewRes->SetName(_Key);
		NewRes->SetType();
		NewRes->SetPathkey(_PathKey);
		NewRes->SetPath(Path.c_str());

		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}

		ResMap.insert(std::unordered_map<std::wstring, Autoptr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}

	static std::vector<Autoptr<Res>> GetAllRes()
	{
		std::vector<Autoptr<Res>> Vec;

		std::unordered_map<std::wstring, Autoptr<Res>>::iterator StartIter = ResMap.begin();
		std::unordered_map<std::wstring, Autoptr<Res>>::iterator EndIter = ResMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			Vec.push_back(StartIter->second);
		}

		return Vec;
	}
private:
	ResourceSystem() {}
	virtual ~ResourceSystem() = 0 {}
};

template<typename Res>
std::unordered_map<std::wstring, Autoptr<Res>> ResourceSystem<Res>::ResMap;

template<typename Res>
std::unordered_map<std::wstring, std::vector<Autoptr<Res>>> ResourceSystem<Res>::MultiResMap;