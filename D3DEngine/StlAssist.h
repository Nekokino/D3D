#pragma once

template<typename RETURN, typename MAP, typename KEY>
RETURN MapFind(MAP& _map, const KEY& _key)
{
	MAP::iterator FindIter = _map.find(_key);
	if (FindIter == _map.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

template<typename MAP, typename KEY>
bool BoolMapFind(MAP& _map, const KEY& _key)
{
	MAP::iterator FindIter = _map.find(_key);
	if (FindIter == _map.end())
	{
		return false;
	}

	return true;
}

template<typename STL, typename KEY>
void StlErase(STL& _stl, const KEY& _key)
{
	STL::iterator FindIter = _stl.find(_key);
	if (FindIter == _stl.end())
	{
		return;
	}

	_stl.erase(FindIter);
}