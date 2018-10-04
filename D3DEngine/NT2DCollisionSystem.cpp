#include "PreCom.h"
#include "NT2DCollisionSystem.h"




NT2DCollisionSystem::NT2DCollisionSystem()
{
}


NT2DCollisionSystem::~NT2DCollisionSystem()
{
}

void NT2DCollisionSystem::Progress()
{
	LinkStartIter = LinkSet.begin();
	LinkEndIter = LinkSet.end();

	for (; LinkStartIter != LinkEndIter; ++LinkStartIter)
	{
		NTIndex Tmp = (*LinkStartIter);

		LeftGroupIter = _2DColMap.find(Tmp.left);
		RightGroupIter = _2DColMap.find(Tmp.right);

		if (LeftGroupIter == _2DColMap.end() || RightGroupIter == _2DColMap.end())
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

void NT2DCollisionSystem::Release()
{
	LeftGroupIter = _2DColMap.begin();
	RightGroupIter = _2DColMap.end();

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



void NT2DCollisionSystem::Push(Autoptr<NTCollisionComponent> _Col)
{
	if (nullptr == _Col)
	{
		tassert(true);
		return;
	}
	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator FindIter = _2DColMap.find(_Col->Order);

	if (FindIter == _2DColMap.end())
	{
		_2DColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Col->Order, std::list<Autoptr<NTCollisionComponent>>()));
		FindIter = _2DColMap.find(_Col->Order);
	}

	FindIter->second.push_back(_Col);
}

void NT2DCollisionSystem::Link(int _Value)
{
	Link(_Value, _Value);
}

void NT2DCollisionSystem::Link(int _Left, int _Right)
{
	NTIndex Index = NTIndex(_Left, _Right);

	std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::iterator FindIter;

	FindIter = _2DColMap.find(_Left);
	if (FindIter == _2DColMap.end())
	{
		_2DColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Left, std::list<Autoptr<NTCollisionComponent>>()));
	}

	FindIter = _2DColMap.find(_Right);
	if (FindIter == _2DColMap.end())
	{
		_2DColMap.insert(std::unordered_map<int, std::list<Autoptr<NTCollisionComponent>>>::value_type(_Right, std::list<Autoptr<NTCollisionComponent>>()));
	}

	std::set<__int64>::iterator FindIter2 = LinkSet.find(Index);

	if (FindIter2 != LinkSet.end())
	{
		return;
	}

	LinkSet.insert(Index);
}

void NT2DCollisionSystem::PushOverCollider2D(Autoptr<NT2DCollisionBase> _Collider)
{

}


