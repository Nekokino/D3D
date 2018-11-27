#include "PreCom.h"
#include "NT2DCollision.h"
#include "NTScene.h"
#include "NTTransform.h"
#include "DebugFunc.h"


NT2DCollision::NT2DCollision()
{
	_2DCollisionFigure = CreateCollisionFigure<_2DColFigure>(COLTYPE::CT_2D_RECT);
}


NT2DCollision::~NT2DCollision()
{
	if (_2DCollisionFigure != nullptr)
	{
		delete _2DCollisionFigure;
		_2DCollisionFigure = nullptr;
	}
}

bool NT2DCollision::Init(int _Order)
{
	Order = _Order;

	GetScene()->_2DCollisionSystem.Push(this);

	return true;
}

void NT2DCollision::CollisionFigureUpdate()
{
	_2DCollisionFigure->Vec.Pos = Pivot + Transform->GetWorldPosition();

	if (false == IndSize)
	{
		switch (_2DCollisionFigure->Type)
		{
		case CT_2D_RECT:
			_2DCollisionFigure->Vec.Size.SetFloat2(fabsf(Transform->GetWorldScale().x), fabsf(Transform->GetWorldScale().y));
			break;
		case CT_2D_CIRCLE:
			_2DCollisionFigure->Vec.Vec3.Radius = fabsf(Transform->GetWorldScale().x) * 0.5f;
		default:
			break;
		}
	}
	else
	{
		switch (_2DCollisionFigure->Type)
		{
		case CT_2D_RECT:
			_2DCollisionFigure->Vec.Size.SetFloat2(fabsf(Size.x), fabsf(Size.y));
			break;
		case CT_2D_CIRCLE:
			_2DCollisionFigure->Vec.Vec3.Radius = (fabsf(Size.x) * 0.5f);
		default:
			break;
		}
	}
}

void NT2DCollision::SetMode(COLTYPE _Type)
{
	switch (_Type)
	{
	case CT_2D_POINT:
	case CT_2D_RECT:
	case CT_2D_CIRCLE:
		_2DCollisionFigure->Type = _Type;
		break;
	default:
		break;
	}
}

void NT2DCollision::DbgRender()
{
	switch (_2DCollisionFigure->Type)
	{
	case CT_2D_RECT:
		DebugFunc::DrawRect(_2DCollisionFigure->Vec);
		break;
	case CT_2D_CIRCLE:
		DebugFunc::DrawCircle(_2DCollisionFigure->Vec.Vec3);
	default:
		break;
	}
}