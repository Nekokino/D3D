#include "PreCom.h"
#include "NTCollisionSystem.h"




NTCollisionSystem::NTCollisionSystem()
{
}


NTCollisionSystem::~NTCollisionSystem()
{
}

void NTCollisionSystem::Progress()
{
	LinkStartIter = LinkSet.begin();
	LinkEndIter = LinkSet.end();

	for (; LinkStartIter != LinkEndIter; ++LinkStartIter)
	{
		NTIndex Tmp = (*LinkStartIter);

		LeftGroupIter = ColMap.find(Tmp.left);
		RightGroupIter = ColMap.find(Tmp.right);

		if (LeftGroupIter == ColMap.end() || RightGroupIter == ColMap.end())
		{
			continue;
		}

		if (0 >= LeftGroupIter->second.size() || 0 >= RightGroupIter->second.size())
		{
			continue;
		}

		LeftStartIter = LeftGroupIter->second.begin();
		LeftEndIter = LeftGroupIter->second.end();

		for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		{
			(*LeftStartIter)->CollisionFigureUpdate();
		}

		RightStartIter = RightGroupIter->second.begin();
		RightEndIter = RightGroupIter->second.end();

		for (; RightStartIter != RightEndIter; ++RightStartIter)
		{
			(*RightStartIter)->CollisionFigureUpdate();
		}

		if (Tmp.left == Tmp.right)
		{
			// 자기 자신과 충돌할떄
		}
		else
		{
			LeftStartIter = LeftGroupIter->second.begin();
			LeftEndIter = LeftGroupIter->second.end();

			for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
			{
				if (false == (*LeftStartIter)->IsUpdate())
				{
					continue;
				}

				RightStartIter = RightGroupIter->second.begin();
				RightEndIter = RightGroupIter->second.end();

				for (; RightStartIter != RightEndIter; ++RightStartIter)
				{
					if (false == (*LeftStartIter)->IsUpdate())
					{
						continue;
					}

					(*LeftStartIter)->ColCheck((*RightStartIter));
				}
			}
		}
	}
}

void NTCollisionSystem::Release()
{
	LeftGroupIter = ColMap.begin();
	RightGroupIter = ColMap.end();

	for (; LeftGroupIter != RightGroupIter; ++LeftGroupIter)
	{
		if (LeftGroupIter->second.size() == 0)
		{
			continue;
		}
		LeftStartIter = LeftGroupIter->second.begin();
		LeftEndIter = LeftGroupIter->second.end();
		for (; LeftStartIter != LeftEndIter;)
		{
			if ((*LeftStartIter)->IsDeath())
			{
				(*LeftStartIter)->ClearSet();
				LeftStartIter = LeftGroupIter->second.erase(LeftStartIter);
			}
			else
				++LeftStartIter;
		}
	}
}



void NTCollisionSystem::Push(Autoptr<NTCollisionComponent> _Col)
{
	if (nullptr == _Col)
	{
		tassert(true);
		return;
	}
	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator FindIter = ColMap.find(_Col->Order);

	if (FindIter == ColMap.end())
	{
		ColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Col->Order, std::list<Autoptr<NTCollisionComponent>>()));
		FindIter = ColMap.find(_Col->Order);
	}

	FindIter->second.push_back(_Col);
}



void NTCollisionSystem::Link(int _Value)
{
	Link(_Value, _Value);
}

void NTCollisionSystem::Link(int _Left, int _Right)
{
	NTIndex Index = NTIndex(_Left, _Right);

	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator FindIter;

	FindIter = ColMap.find(_Left);
	if (FindIter == ColMap.end())
	{
		ColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Left, std::list<Autoptr<NTCollisionComponent>>()));
	}

	FindIter = ColMap.find(_Right);
	if (FindIter == ColMap.end())
	{
		ColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Right, std::list<Autoptr<NTCollisionComponent>>()));
	}

	std::set<__int64>::iterator FindIter2 = LinkSet.find(Index);

	if (FindIter2 != LinkSet.end())
	{
		return;
	}

	LinkSet.insert(Index);
}

void NTCollisionSystem::PushOverCollider2D(Autoptr<NT2DCollisionBase> _Collider)
{

}


std::list<Autoptr<NTCollisionComponent>> NTCollisionSystem::CheckCollisionFigureList(int _Order, NTCollisionFigure * _Figure)
{
	std::list<Autoptr<NTCollisionComponent>> Return;

	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator UpdateIter = ColMap.find(_Order);

	if (UpdateIter == ColMap.end())
	{
		return Return;
	}

	std::list<Autoptr<NTCollisionComponent>>::iterator ListStartIter = UpdateIter->second.begin();
	std::list<Autoptr<NTCollisionComponent>>::iterator ListEndIter = UpdateIter->second.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (true == (*ListStartIter)->FigureCheck(_Figure))
		{
			Return.push_back((*ListStartIter));
		}
	}
	
	return Return;
}

Autoptr<NTCollisionComponent> NTCollisionSystem::CheckCollisionComponent(int _Order, NTCollisionFigure * _Figure)
{
	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator CheckIter = ColMap.find(_Order);

	if (CheckIter == ColMap.end())
	{
		return nullptr;
	}

	std::list<Autoptr<NTCollisionComponent>>::iterator StartIter = CheckIter->second.begin();
	std::list<Autoptr<NTCollisionComponent>>::iterator EndIter = CheckIter->second.end();

	for (; StartIter != EndIter; ++EndIter)
	{
		if (true == (*StartIter)->FigureCheck(_Figure))
		{
			return (*StartIter);
		}
	}

	return nullptr;
}