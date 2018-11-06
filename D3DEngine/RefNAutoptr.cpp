#include "PreCom.h"
#include "RefNAutoptr.h"


RefCounter::RefCounter() : Counter(0)
{
}

RefCounter::RefCounter(const RefCounter & _Other) : Counter(0)
{
}


RefCounter::~RefCounter()
{
}

void RefCounter::AddRef()
{
	++Counter;
}

void RefCounter::DecRef()
{
	--Counter;

	if (0 >= Counter)
	{
		delete this;
	}
}