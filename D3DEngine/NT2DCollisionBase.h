#pragma once
#include "NTCollisionComponent.h"

class NT2DCollisionBase : public NTCollisionComponent
{
protected:
	bool IndSize;

	NTVEC2 Pivot;
	NTVEC2 Size;

public:
	void SetPosition();
	void SetSize();

	NTVEC GetIndVec()
	{
		return NTVEC{ Pivot, Size };
	}
public:
	NT2DCollisionBase();
	~NT2DCollisionBase();
};

