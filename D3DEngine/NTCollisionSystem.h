#pragma once
#include <set>
#include <list>
#include <unordered_map>
#include "NTCollisionComponent.h"
#include "DirectHeader.h"
#include "NT2DCollisionBase.h"

class NTScene;
class NTCollisionSystem
{
public:
	friend NTScene;
private:
	std::set<__int64> LinkSet;
	std::set<__int64>::iterator LinkStartIter;
	std::set<__int64>::iterator LinkEndIter;

	std::unordered_map <int, std::list<Autoptr<NTCollisionComponent>>> ColMap;
	std::unordered_map <int, std::list<Autoptr<NTCollisionComponent>>>::iterator LeftGroupIter;
	std::unordered_map <int, std::list<Autoptr<NTCollisionComponent>>>::iterator RightGroupIter;

	std::list<Autoptr<NTCollisionComponent>>::iterator LeftStartIter;
	std::list<Autoptr<NTCollisionComponent>>::iterator LeftEndIter;
	std::list<Autoptr<NTCollisionComponent>>::iterator RightStartIter;
	std::list<Autoptr<NTCollisionComponent>>::iterator RightEndIter;
public:
	void Link(int _Value);
	void Link(int _Left, int _Right);
	void Push(Autoptr<NTCollisionComponent> _Col);
	std::list<Autoptr<NTCollisionComponent>> CheckCollisionFigureList(int _Order, NTCollisionFigure* _Figure);
	Autoptr<NTCollisionComponent> CheckCollisionComponent(int _Order, NTCollisionFigure* _Figure);

private:
	void Progress();
	void Release();
private:
	void PushOverCollider2D(Autoptr<NT2DCollisionBase> _Collider);

public:
	NTCollisionSystem();
	~NTCollisionSystem();
};

