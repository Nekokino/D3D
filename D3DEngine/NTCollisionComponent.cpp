#include "PreCom.h"
#include "NTCollisionComponent.h"

bool(*NTCollisionComponent::ColFunction[CT_MAX][CT_MAX])(NTCollisionFigure* _Left, NTCollisionFigure* _Right);

NTCollisionComponent::NTCollisionComponent()
{
}


NTCollisionComponent::~NTCollisionComponent()
{
}

void NTCollisionComponent::CollsionInit()
{
	for (size_t y = 0; y < CT_MAX; y++)
	{
		for (size_t x = 0; x < CT_MAX; x++)
		{
			ColFunction[y][x] = nullptr;
		}
	}

	ColFunction[CT_2D_RECT][CT_2D_RECT] = &NTCollisionFunction::R2RFigure;
	ColFunction[CT_2D_RECT][CT_2D_POINT] = &NTCollisionFunction::R2PFigure;
	ColFunction[CT_2D_POINT][CT_2D_RECT] = &NTCollisionFunction::P2RFigure;
	ColFunction[CT_2D_CIRCLE][CT_2D_CIRCLE] = &NTCollisionFunction::C2CFigure;
	ColFunction[CT_2D_CIRCLE][CT_2D_POINT] = &NTCollisionFunction::C2PFigure;
	ColFunction[CT_2D_POINT][CT_2D_CIRCLE] = &NTCollisionFunction::P2CFigure;
	ColFunction[CT_2D_RECT][CT_2D_CIRCLE] = &NTCollisionFunction::R2CFigure;
	ColFunction[CT_2D_CIRCLE][CT_2D_RECT] = &NTCollisionFunction::C2RFigure;
}

bool NTCollisionComponent::Init()
{
	return true;
}

void NTCollisionComponent::CollisionFigureUpdate()
{

}

bool NTCollisionComponent::ColCheck(NTCollisionComponent* _Col)
{
	if (nullptr == ColFunction)
	{
		return false;
	}

	bool Check = ColFunction[Figure->Type][_Col->Figure->Type](Figure, _Col->Figure);

	if (true == Check)
	{
		FindIter = CollisionSet.find(_Col);
		if (FindIter == CollisionSet.end())
		{
			CollisionSet.insert(_Col);
			_Col->CollisionSet.insert(this);
			CallEnterList(_Col);
		}
		else
		{
			CallStayList(_Col);
		}
	}
	else
	{
		FindIter = CollisionSet.find(_Col);
		if (FindIter != CollisionSet.end())
		{
			CollisionSet.erase(_Col);
			_Col->CollisionSet.erase(this);
			CallExitList(_Col);
		}
	}

	return false;
}

void NTCollisionComponent::CallEnterList(NTCollisionComponent * _Right)
{
	EnterStartIter = EnterFuncList.begin();
	EnterEndIter = EnterFuncList.end();

	for (; EnterStartIter != EnterEndIter; ++EnterStartIter)
	{
		(*EnterStartIter)(this, _Right);
	}
}

void NTCollisionComponent::CallStayList(NTCollisionComponent * _Right)
{
	StayStartIter = StayFuncList.begin();
	StayEndIter = StayFuncList.end();

	for (; StayStartIter != StayEndIter; ++StayStartIter)
	{
		(*StayStartIter)(this, _Right);
	}
}

void NTCollisionComponent::CallExitList(NTCollisionComponent * _Right)
{
	ExitStartIter = ExitFuncList.begin();
	ExitEndIter = ExitFuncList.end();

	for (; ExitStartIter != ExitEndIter; ++ExitStartIter)
	{
		(*ExitStartIter)(this, _Right);
	}
}

void NTCollisionComponent::ClearSet() // 나를 set에서 들고있는 놈들은 다 날 조져라
{
	std::set<NTCollisionComponent*>::iterator StartIter = CollisionSet.begin();
	std::set<NTCollisionComponent*>::iterator EndIter = CollisionSet.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->CollisionSet.erase(this);
	}
}