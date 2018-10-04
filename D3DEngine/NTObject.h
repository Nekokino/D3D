#pragma once
#include "NTWinParent.h"
#include "NTSceneParent.h"
#include "NTSwitcher.h"

#include <list>

enum OVERSCENETYPE
{
	OST_NONE,
	OST_ALL,
	OST_CERTAIN,
};

class NTScene;
class NTComponent;
class NTTransform;
class NTRenderSystem;
class NT2DCollisionSystem;
class NTObject : public NTWinParent, public NTSceneParent, public NTSwitcher
{
public:
	friend NTScene;

private:
	std::list<Autoptr<NTComponent>> ComList;
	std::list<Autoptr<NTComponent>>::iterator ListStartIter;
	std::list<Autoptr<NTComponent>>::iterator ListEndIter;

	int Order;

private:
	OVERSCENETYPE OSType;

public:
	void SetOverObject();
	
	OVERSCENETYPE GetOSType()
	{
		return OSType;
	}

public:
	int GetOrder()
	{
		return Order;
	}

private:
	void SetOrder(int _Order)
	{
		Order = _Order;
	}

private:
	void PreUpdate() override;
	void MainUpdate() override;
	void AfterUpdate() override;
	void DbgUpdate() override;
	void DbgRender() override;
	void FinalUpdate();
	void EndUpdate();

	void Release();

public:
	void Death() override;

private:
	NTObject* Parent; // 상호참조 문제때문에 단순 포인터로 가지고있음.
	std::list<Autoptr<NTObject>> ChildList;
	std::list<Autoptr<NTObject>>::iterator ChildStartIter;
	std::list<Autoptr<NTObject>>::iterator ChildEndIter;

	void TransferChild() {}

public:
	NTObject* GetParent()
	{
		return Parent;
	}

private:
	void Detach();

public:
	void AddChild(Autoptr<NTObject> _Obj, bool _World = true);
	Autoptr<NTObject> FindChild(const wchar_t* _Name);

public:
	friend NTTransform;
	NTTransform* Transform;

	NTTransform* GetTransform()
	{
		return Transform;
	}

private:
	void SetTransform(NTTransform* _Transform)
	{
		Transform = _Transform;
	}

public:
	template<typename Component>
	Autoptr<Component> AddComponent()
	{
		if (false == Component::IsMulti(this))
		{
			return nullptr;
		}

		Component* NewCom = new Component;
		NewCom->SetType();
		NewCom->SetObject(this);
		NewCom->SetNTWindow(GetNTWindow());
		NewCom->SetScene(GetScene());
		NewCom->InitComponent();

		if (false == NewCom->Init())
		{
			delete NewCom;
			return nullptr;
		}

		ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Component, typename T1>
	Autoptr<Component> AddComponent(T1 _1)
	{
		if (false == Component::IsMulti(this))
		{
			return nullptr;
		}

		Component* NewCom = new Component;
		NewCom->SetType();
		NewCom->SetObject(this);
		NewCom->SetNTWindow(GetNTWindow());
		NewCom->SetScene(GetScene());
		NewCom->InitComponent();

		if (false == NewCom->Init(_1))
		{
			delete NewCom;
			return nullptr;
		}

		ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Component, typename T1, typename T2>
	Autoptr<Component> AddComponent(T1 _1, T2 _2)
	{
		if (false == Component::IsMulti(this))
		{
			return nullptr;
		}

		Component* NewCom = new Component;
		NewCom->SetType();
		NewCom->SetObject(this);
		NewCom->SetNTWindow(GetNTWindow());
		NewCom->SetScene(GetScene());
		NewCom->InitComponent();

		if (false == NewCom->Init(_1, _2))
		{
			delete NewCom;
			return nullptr;
		}

		ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Component, typename T1, typename T2, typename T3>
	Autoptr<Component> AddComponent(T1 _1, T2 _2, T3 _3)
	{
		if (false == Component::IsMulti(this))
		{
			return nullptr;
		}

		Component* NewCom = new Component;
		NewCom->SetType();
		NewCom->SetObject(this);
		NewCom->SetNTWindow(GetNTWindow());
		NewCom->SetScene(GetScene());
		NewCom->InitComponent();

		if (false == NewCom->Init(_1, _2, _3))
		{
			delete NewCom;
			return nullptr;
		}

		ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Component>
	Autoptr<Component> GetComponent(int _index = 0)
	{
		int Count = 0;
		ListStartIter = ComList.begin();
		ListEndIter = ComList.end();

		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			if (true == (*ListStartIter)->IsEqual<Component>())
			{
				if (Count == _index)
				{
					return (*ListStartIter);
				}
				else
					++Count;
				
			}
		}

		return nullptr;
	}

private:
	void PushOverRenderer(NTRenderSystem* _RendSys);
	void PushOverCollider2D(NT2DCollisionSystem* _2DColSys);

private:
	NTObject();
	~NTObject();
};

