#pragma once
#include "NTSwitcher.h"
#include "NTObjectParent.h"
#include "NTWinParent.h"
#include "NTObject.h"
#include "NTSceneParent.h"

class NTComponent : public NTSwitcher, public NTObjectParent, public NTWinParent, public NTSceneParent
{
public:
	friend NTObject;

private:
	static bool IsMulti(NTObject* _Obj);

public:
	template<typename T>
	Autoptr<T> GetComponent()
	{
		return GetNTObject()->GetComponent<T>();
	}

	template<typename T>
	Autoptr<T> AddComponent()
	{
		return GetNTObject()->AddComponent<T>();
	}

	template<typename T, typename V1>
	Autoptr<T> AddComponent(V1 _1)
	{
		return GetNTObject()->AddComponent<T>(_1);
	}

	template<typename T, typename V1, typename V2>
	Autoptr<T> AddComponent(V1 _1, V2 _2)
	{
		return GetNTObject()->AddComponent<T>(_1, _2);
	}
	
	template<typename T, typename V1, typename V2, typename V3>
	Autoptr<T> AddComponent(V1 _1, V2 _2, V3 _3)
	{
		return GetNTObject()->AddComponent<T>(_1, _2, _3);
	}
public:
	virtual void InitComponent();

	virtual void FinalUpdate();
	virtual void EndUpdate();
protected:
	NTComponent();
	~NTComponent();
};

