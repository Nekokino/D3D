#pragma once
#include "PreCom.h"
#include "NTStCom.h"
#include "MathSystem.h"
#include <set>
#include <functional>
#include <DirectXCollision.h>

enum COLTYPE
{
	CT_2D_POINT,
	CT_2D_RECT,
	CT_2D_CIRCLE,
	CT_2D_POLYGON,
	CT_3D_PLANE,
	CT_3D_POINT,
	CT_3D_SPHERE,
	CT_3D_RAY,
	CT_3D_AABB,
	CT_3D_OBB,
	CT_MAX
};

class NTCollisionComponent;
class NTCollisionFunction;
class NTCollisionFigure
{
public:
	friend NTCollisionComponent;
	friend NTCollisionFunction;

public:
	COLTYPE Type;

protected:
	NTCollisionFigure() {}
	virtual ~NTCollisionFigure() = 0 {}
};

class _2DColFigure :  public NTCollisionFigure
{
public:
	friend NTCollisionComponent;

public:
	NTVEC Vec;

public:
	_2DColFigure() {}
	virtual ~_2DColFigure() {}
};

class NTSphereData : public NTCollisionFigure
{
public:
	DirectX::BoundingSphere Sphere;

public:
	NTSphereData() {}
	virtual ~NTSphereData() {}
};

class NTRayData : public NTCollisionFigure
{
public:
	NTVEC Origin;
	NTVEC Direction;
	float Dis;

public:
	NTRayData() {}
	~NTRayData() {}
};

class NTCollisionFunction
{
public:
	static bool R2RFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}

		if (_Left->Type != COLTYPE::CT_2D_RECT || _Right->Type != COLTYPE::CT_2D_RECT)
		{
			return false;
		}
#endif

		return MathSystem::R2RCol(((_2DColFigure*)_Left)->Vec, ((_2DColFigure*)_Right)->Vec);
	}
	static bool C2CFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}

		if (_Left->Type != COLTYPE::CT_2D_CIRCLE || _Right->Type != COLTYPE::CT_2D_CIRCLE)
		{
			return false;
		}
#endif

		return MathSystem::C2CCol(((_2DColFigure*)_Left)->Vec.Vec3, ((_2DColFigure*)_Right)->Vec.Vec3);
	}
	static bool C2PFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}

		if (_Left->Type != COLTYPE::CT_2D_CIRCLE || _Right->Type != COLTYPE::CT_2D_POINT)
		{
			return false;
		}
#endif

		return MathSystem::C2PCol(((_2DColFigure*)_Left)->Vec.Vec3, ((_2DColFigure*)_Right)->Vec.Pos);
	}
	static bool P2CFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
		return C2PFigure(_Right, _Left);
	}
	static bool R2PFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}

		if (_Left->Type != COLTYPE::CT_2D_CIRCLE || _Right->Type != COLTYPE::CT_2D_POINT)
		{
			return false;
		}
#endif

		return MathSystem::R2PCol(((_2DColFigure*)_Left)->Vec, ((_2DColFigure*)_Right)->Vec.Vec2);
	}
	static bool P2RFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
		return R2PFigure(_Right, _Left);
	}
	static bool C2RFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->Type != COLTYPE::CT_2D_CIRCLE || _Right->Type != COLTYPE::CT_2D_RECT)
		{
			return false;
		}
#endif
		return MathSystem::C2RCol(((_2DColFigure*)_Left)->Vec.Vec3, ((_2DColFigure*)_Right)->Vec);
	}
	static bool R2CFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
		return C2RFigure(_Right, _Left);
	}

	static bool S2SFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->Type != COLTYPE::CT_3D_SPHERE || _Right->Type != COLTYPE::CT_3D_SPHERE)
		{
			return false;
		}
#endif
		return MathSystem::S2SCol(((NTSphereData*)_Left)->Sphere, ((NTSphereData*)_Right)->Sphere);
	}

	static bool S2RayFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->Type != COLTYPE::CT_3D_SPHERE || _Right->Type != COLTYPE::CT_3D_RAY)
		{
			return false;
		}
#endif
		NTRayData* Data = (NTRayData*)_Right;
		return MathSystem::S2RayCol(((NTSphereData*)_Left)->Sphere, Data->Origin, Data->Direction, Data->Dis);
	}

	static bool Ray2SFigure(NTCollisionFigure* _Left, NTCollisionFigure* _Right)
	{
		return S2RayFigure(_Right, _Left);
	}
};

class WinCore;
class NTCollisionSystem;
class NTCollisionComponent : public NTStCom
{
public:
	friend WinCore;
	friend NTCollisionSystem;

private:
	static void CollsionInit();
	static bool(*ColFunction[CT_MAX][CT_MAX])(NTCollisionFigure* _Left, NTCollisionFigure* _Right);

private:
	NTCollisionFigure* Figure;

protected:
	int Order;

public:
	std::set<NTCollisionComponent*> CollisionSet;
	std::set<NTCollisionComponent*>::iterator FindIter;

public:
	virtual bool Init();
	virtual void CollisionFigureUpdate();

protected:
	template<typename T>
	T* CreateCollisionFigure(COLTYPE _Type)
	{
		T* tmp = new T();
		tmp->Type = _Type;
		Figure = tmp;
		return tmp;
	}

public:
	void ColCheck(NTCollisionComponent* _Col);
	bool FigureCheck(NTCollisionFigure* _Figure);


	void ClearSet();

private:
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>> EnterFuncList;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator EnterStartIter;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator EnterEndIter;

	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>> StayFuncList;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator StayStartIter;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator StayEndIter;

	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>> ExitFuncList;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator ExitStartIter;
	std::list<std::function<void(NTCollisionComponent* _Left, NTCollisionComponent* _Right)>>::iterator ExitEndIter;

public:
	template<typename T>
	void EnterFunc(T* _Obj, void(T::*_ptr)(NTCollisionComponent*, NTCollisionComponent*))
	{
		if (nullptr == _ptr)
		{
			return;
		}

		if (nullptr == _Obj)
		{
			return;
		}

		EnterFuncList.push_back(std::bind(_ptr, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void StayFunc(T* _Obj, void(T::*_ptr)(NTCollisionComponent*, NTCollisionComponent*))
	{
		if (nullptr == _ptr)
		{
			return;
		}

		if (nullptr == _Obj)
		{
			return;
		}

		StayFuncList.push_back(std::bind(_ptr, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void ExitFunc(T* _Obj, void(T::*_ptr)(NTCollisionComponent*, NTCollisionComponent*))
	{
		if (nullptr == _ptr)
		{
			return;
		}

		if (nullptr == _Obj)
		{
			return;
		}

		ExitFuncList.push_back(std::bind(_ptr, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	void CallEnterList(NTCollisionComponent* _Right);
	void CallStayList(NTCollisionComponent* _Right);
	void CallExitList(NTCollisionComponent* _Right);
public:
	NTCollisionComponent();
	virtual ~NTCollisionComponent();
};

