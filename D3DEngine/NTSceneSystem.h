#pragma once
#include <unordered_map>
#include <string>
#include "NTScene.h"

class NTObject;
class NTWindow;
class NTSceneSystem : public NTWinParent
{
public:
	friend NTWindow;
	friend NTObject;

private:
	std::list<Autoptr<NTObject>> OverObjectList;

public:
	void OverCheck(NTScene* _Scene);

public:
	bool FindOverObj(Autoptr<NTObject> _Obj);

private:
	std::unordered_map<std::wstring, Autoptr<NTScene>> SceneMap;
	Autoptr<NTScene> CurScene;
	Autoptr<NTScene> NextScene;

public:
	template<typename Builder, typename Updater>
	Autoptr<NTScene> CreateScene(const wchar_t* _Name, bool _Build = true)
	{
		if (nullptr != FindScene(_Name))
		{
			return nullptr;
		}

		NTScene* NewScene = new NTScene();
		NewScene->SetSceneSystem(this);
		NewScene->SetNTWindow(GetNTWindow());
		NewScene->CreateBuilder<Builder>();
		NewScene->CreateUpdater<Updater>();
		NewScene->SetName(_Name);
		NewScene->SetType();

		OverCheck(NewScene);

		if (true == _Build)
		{
			NewScene->Build();
		}

		SceneMap.insert(std::unordered_map<std::wstring, Autoptr<NTScene>>::value_type(_Name, NewScene));
		return NewScene;
	}

	Autoptr<NTScene> CreateScene(const wchar_t* _Name);

	Autoptr<NTScene> FindScene(const wchar_t* _Name);
	void ChangeScene(const wchar_t* _Name);

	Autoptr<NTScene> GetCurScene()
	{
		return CurScene;
	}

	Autoptr<NTScene> GetNextScene()
	{
		return NextScene;
	}

private:
	void Progress();
	void Render();
	void PreProgress();
	void Release();

public:
	NTSceneSystem(NTWindow* _Window);
	~NTSceneSystem();
};

