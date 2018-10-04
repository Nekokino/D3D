#pragma once
#include "NT2DCollisionBase.h"

class NT2DCollision : public NT2DCollisionBase
{
private:
	_2DColFigure* _2DCollisionFigure;

public:
	bool Init(int _Order = 0);

	void CollisionFigureUpdate() override;
	void SetMode(COLTYPE _Type);
	void SetSize(NTVEC2 _Size)
	{
		IndSize = true;
		_2DCollisionFigure->Vec.Size = _Size;
	}

	void SetPos(NTVEC2 _Pos)
	{
		IndSize = true;
		_2DCollisionFigure->Vec.Pos = _Pos;
	}

	void SetRect(NTVEC _Vec)
	{
		IndSize = true;
		Pivot = _Vec.Pos;
		Size = _Vec.Size;
	}

	void SetRadius(float _Diameter)
	{
		IndSize = true;
		Size = _Diameter;
	}

	void SetMove(NTVEC _Vec)
	{
		_2DCollisionFigure->Vec += _Vec;
	}

	NTRECT GetRect()
	{
		return{ Pivot, Size };
	}

	NTVEC GetColliderVec()
	{
		return _2DCollisionFigure->Vec;
	}

public:
	void DbgRender() override;
public:
	NT2DCollision();
	~NT2DCollision();
};

