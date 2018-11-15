#include "PreCom.h"
#include "ResourceSystem.h"

#include <Windows.h>
#include "NTImage.h"

std::unordered_map<std::wstring, std::wstring> PathSystem::PathMap;
std::wstring PathSystem::Root;

void PathSystem::Init()
{
	wchar_t Arr[1024];

	GetCurrentDirectoryW(sizeof(wchar_t) * 1024, Arr);
	Root = Arr;

	size_t CutCount = Root.find_last_of('\\', Root.size());
	Root.replace(CutCount, Root.size(), L"\\Bin\\");
}

void PathSystem::CreatePath(const wchar_t* _Key, const wchar_t* _Folder)
{
	if (nullptr != FindPath(_Key))
	{
		tassert(true);
	}

	std::wstring NewPath = Root + _Folder + L"\\";
	PathMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, NewPath));

}

void PathSystem::CreatePath(const wchar_t* _Key)
{
	CreatePath(_Key, _Key);
}

std::wstring PathSystem::CreateFilePath(const wchar_t* _FolderKey, const wchar_t* _FileName)
{
	std::wstring Path = FindPath(_FolderKey);
	return Path += _FileName;
}

const wchar_t* PathSystem::FindPath(const wchar_t* _Key)
{
	std::unordered_map<std::wstring, std::wstring>::iterator PathIter = PathMap.find(_Key);

	if (PathIter == PathMap.end())
	{
		return nullptr;
	}

	return PathIter->second.c_str();
}

bool PathSystem::IsFile(const wchar_t* _Path)
{
	int Return = _waccess_s(_Path, 0) + 1;

	if (0 == Return)
	{
		return false;
	}

	return true;
}

void PathSystem::CreateAllPath(const wchar_t* _RootPath)
{
	std::wstring FindPath = PathSystem::FindPath(_RootPath);
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
				std::wstring tmp = _RootPath;
				tmp += L"\\";
				tmp += FindData.cFileName;

				if (nullptr == PathSystem::FindPath(FindData.cFileName))
				{
					PathSystem::CreatePath(FindData.cFileName, tmp.c_str());
				}

				CreateAllPathChild(_RootPath, _RootPath, FindData.cFileName);
			}
		}
	}

	FindClose(Finder);
}

void PathSystem::CreateAllPathChild(const wchar_t* _RootPath, const wchar_t* _UpperPath, const wchar_t* _Path)
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
				std::wstring tmp = PathSystem::FindPath(_UpperPath);
				size_t Cut = tmp.find(_RootPath);
				tmp.replace(0, Cut, L"");
				tmp += _Path;
				tmp += L"\\";
				tmp += FindData.cFileName;

				if (nullptr == PathSystem::FindPath(FindData.cFileName))
				{
					PathSystem::CreatePath(FindData.cFileName, tmp.c_str());
				}

				CreateAllPathChild(_RootPath, _Path, FindData.cFileName);
			}
		}
	}

	FindClose(Finder);
}

std::wstring PathSystem::FindPathString(const wchar_t * _Key)
{
	return FindPath(_Key);
}
