#include "PreCom.h"
#include "NTObject.h"
#include "NTScene.h"
#include "StlAssist.h"
#include "NTTransform.h"
#include "NTCamera.h"

NTScene::NTScene() : MainCamera(nullptr)
{
	SetType();
}


NTScene::~NTScene()
{
}


void NTScene::Progress()
{
	if (nullptr != SceneUpdater)
	{
		SceneUpdater->SceneUpdate();
	}

	PreUpdate();
	MainUpdate();
	AfterUpdate();
	FinalUpdate();
	DbgUpdate();

	_2DCollsionSystem.Progress();
}

void NTScene::PreUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->PreUpdate();
		}
	}
}

void NTScene::MainUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->MainUpdate();
		}
	}
}

void NTScene::AfterUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->AfterUpdate();
		}
	}
}

void NTScene::DbgUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->DbgUpdate();
		}
	}
}

void NTScene::Render()
{
	RenderSystem.Render();
}

void NTScene::DbgRender()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->DbgRender();
		}
	}
}

void NTScene::Release()
{
	RenderSystem.Release();
	_2DCollsionSystem.Release();

	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter;)
		{
			if (false == (*ListStartIter)->IsDeath())
			{
				(*ListStartIter)->Release();
				++ListStartIter;
				continue;
			}

			ListStartIter = MapStartIter->second.erase(ListStartIter);
		}
	}
}

void NTScene::Build()
{
	if (nullptr != SceneBuilder)
	{
		SceneBuilder->SceneBuild();
		SceneBuilder->bBuild = true;

		if (nullptr != SceneUpdater)
		{
			SceneUpdater->SceneStart();
		}
	}
}

std::unordered_map<int, std::list<Autoptr<NTObject>>>::iterator ObjIter;
Autoptr<NTObject> NTScene::CreateObject(const wchar_t* _Name, int _Order)
{
	if (false == BoolMapFind(ObjectMap, _Order))
	{
		ObjectMap.insert(std::unordered_map<int, std::list<Autoptr<NTObject>>>::value_type(_Order, std::list<Autoptr<NTObject>>()));
	}

	ObjIter = ObjectMap.find(_Order);
	NTObject* NewObj = new NTObject();
	NewObj->SetName(_Name);
	NewObj->SetOrder(_Order);
	NewObj->SetType();
	NewObj->SetNTWindow(GetNTWindow());
	NewObj->SetScene(this);
	NewObj->AddComponent<NTTransform>();

	ObjIter->second.push_back(NewObj);

	return NewObj;
}

Autoptr<NTObject> NTScene::FindObject(const wchar_t* _Name, int _Order)
{
	if (false == BoolMapFind(ObjectMap, _Order))
	{
		return nullptr;
	}

	ObjIter = ObjectMap.find(_Order);

	std::list<Autoptr<NTObject>>::iterator StartIter = ObjIter->second.begin();
	std::list<Autoptr<NTObject>>::iterator EndIter = ObjIter->second.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (lstrcmpW(_Name, (*StartIter)->GetName()) == 0)
		{
			return (*StartIter);
		}
	}

	return nullptr;
}

void NTScene::PreProgress()
{
	EndUpdate();
}

void NTScene::EndUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->EndUpdate();
		}
	}
}

void NTScene::FinalUpdate()
{
	MapStartIter = ObjectMap.begin();
	MapEndIter = ObjectMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (false == (*ListStartIter)->IsUpdate())
			{
				continue;
			}

			(*ListStartIter)->FinalUpdate();
		}
	}
}

void NTScene::MoveObject(Autoptr<NTObject> _Obj)
{
	ObjIter = ObjectMap.find(_Obj->GetOrder());

	tassert(ObjIter == ObjectMap.end());
	if (ObjIter == ObjectMap.end())
	{
		return;
	}

	ObjIter->second.push_back(_Obj);

	return;
}

bool NTScene::EraseObject(Autoptr<NTObject> _Obj)
{
	ObjIter = ObjectMap.find(_Obj->GetOrder());

	tassert(ObjIter == ObjectMap.end());

	if (ObjIter == ObjectMap.end())
	{
		return false;
	}

	std::list<Autoptr<NTObject>>::iterator ListStartIter = ObjIter->second.begin();
	std::list<Autoptr<NTObject>>::iterator ListEndIter = ObjIter->second.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (_Obj == *ListStartIter)
		{
			ObjIter->second.erase(ListStartIter);
			return true;
		}
	}

	tassert(ObjIter == ObjectMap.end());
	return false;
}

void NTScene::PushOverObject(NTObject * _Obj)
{
	if (false == BoolMapFind(ObjectMap, _Obj->GetOrder()))
	{
		ObjectMap.insert(std::unordered_map<int, std::list<Autoptr<NTObject>>>::value_type(_Obj->GetOrder(), std::list<Autoptr<NTObject>>()));
	}

	ObjIter = ObjectMap.find(_Obj->GetOrder());

	if (ObjIter == ObjectMap.end())
	{
		return;
	}

	ObjIter->second.push_back(_Obj);

	_Obj->PushOverRenderer(&RenderSystem);
	_Obj->PushOverCollider2D(&_2DCollsionSystem);
}

NTCamera * NTScene::GetMainCamera()
{
	return MainCamera;
}