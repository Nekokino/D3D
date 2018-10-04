#include "PreCom.h"
#include "NTObject.h"
#include "NTComponent.h"
#include "NTScene.h"
#include "NTWinShortCut.h"
#include "NTRenderSystem.h"
#include "NT2DCollisionSystem.h"
#include "NT2DCollision.h"
#include "NTRenderer.h"

NTObject::NTObject()
{
}


NTObject::~NTObject()
{
}

void NTObject::SetOverObject()
{
	OSType = OST_ALL;

	if (true == GetNTWindow()->SceneSystem.FindOverObj(this))
	{
		return;
	}

	GetNTWindow()->SceneSystem.OverObjectList.push_back(this);

	std::unordered_map<std::wstring, Autoptr<NTScene>>::iterator StartIter = GetNTWindow()->SceneSystem.SceneMap.begin();
	std::unordered_map<std::wstring, Autoptr<NTScene>>::iterator EndIter = GetNTWindow()->SceneSystem.SceneMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (StartIter->second == GetScene())
		{
			continue;
		}
		StartIter->second->PushOverObject(this);
	}

	
}

void NTObject::PreUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->PreUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->PreUpdate();
	}
}

void NTObject::MainUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->MainUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->MainUpdate();
	}
}

void NTObject::AfterUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->AfterUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->AfterUpdate();
	}
}

void NTObject::DbgUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->DbgUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DbgUpdate();
	}
}

void NTObject::DbgRender()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->DbgRender();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DbgRender();
	}
}

void NTObject::FinalUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->FinalUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->FinalUpdate();
	}
}

void NTObject::EndUpdate()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsUpdate())
		{
			continue;
		}

		(*ListStartIter)->EndUpdate();
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->EndUpdate();
	}
}

void NTObject::Release()
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter;)
	{
		if (true == (*ListStartIter)->IsDeath())
		{
			ListStartIter = ComList.erase(ListStartIter);
		}
		else
		{
			++ListStartIter;
		}
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Release();
	}
}

void NTObject::Death()
{
	NTSwitcher::Death(); // 일단 나도 죽고.

	std::list<Autoptr<NTComponent>>::iterator ComStartIter = ComList.begin();
	std::list<Autoptr<NTComponent>>::iterator ComEndIter = ComList.end();

	for (; ComStartIter != ComEndIter; ++ComStartIter)
	{
		(*ComStartIter)->Death();
	}
}

void NTObject::AddChild(Autoptr<NTObject> _Obj, bool _World)
{
	if (OST_NONE != _Obj->GetOSType())
	{
		return;
	}

	if (false == GetScene()->EraseObject(_Obj))
	{
		return;
	}

	_Obj->Parent = this;

	ChildList.push_back(_Obj);

	if (true == _World && nullptr != GetTransform() && nullptr != _Obj->GetTransform())
	{
		NTVEC OScale = _Obj->GetTransform()->GetLocalScale();
		NTVEC MScale = GetTransform()->GetLocalScale();

		_Obj->GetTransform()->SetLocalScale(OScale / MScale);

		NTVEC OPos = _Obj->GetTransform()->GetLocalPosition();
		NTVEC MPos = GetTransform()->GetLocalPosition();

		_Obj->GetTransform()->SetLocalPosition((OPos - MPos) / GetTransform()->GetLocalScale());

		NTVEC ORot = _Obj->GetTransform()->GetLocalRotation();
		NTVEC MRot = GetTransform()->GetLocalRotation();

		_Obj->GetTransform()->SetLocalRotation(ORot - MRot);
	}
	else if (false == _World && nullptr != GetTransform() && nullptr != _Obj->GetTransform())
	{
		_Obj->GetTransform()->Reset();
	}
}

Autoptr<NTObject> NTObject::FindChild(const wchar_t * _Name)
{
	std::list<Autoptr<NTObject>>::iterator StartIter;
	std::list<Autoptr<NTObject>>::iterator EndIter;
	StartIter = ChildList.begin();
	EndIter = ChildList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (lstrcmpW((*StartIter)->GetName(), _Name) == 0)
		{
			return (*StartIter);
		}
	}

	return nullptr;
}

void NTObject::Detach()
{
	std::list<Autoptr<NTObject>>::iterator StartIter = ChildList.begin();
	std::list<Autoptr<NTObject>>::iterator EndIter = ChildList.end();

	bool TmpCheck = false;

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter) == this)
		{
			Parent->ChildList.erase(StartIter);
			TmpCheck = true;
			break;
		}
	}

	if (false == TmpCheck)
	{
		tassert(true);
		return;
	}

	// 뺀 애를 씬에 넣어저야해여.
}

void NTObject::PushOverRenderer(NTRenderSystem * _RendSys)
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsParent<NTRenderer>())
		{
			continue;
		}
		
		_RendSys->PushOverRenderer((*ListStartIter));
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->PushOverRenderer(_RendSys);
	}
}

void NTObject::PushOverCollider2D(NT2DCollisionSystem * _2DColSys)
{
	ListStartIter = ComList.begin();
	ListEndIter = ComList.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (false == (*ListStartIter)->IsParent<NT2DCollision>())
		{
			continue;
		}

		_2DColSys->Push((*ListStartIter));
	}

	ChildStartIter = ChildList.begin();
	ChildEndIter = ChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->PushOverCollider2D(_2DColSys);
	}
}