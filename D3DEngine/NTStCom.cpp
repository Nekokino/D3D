#include "PreCom.h"
#include "NTStCom.h"


NTStCom::NTStCom() : Transform(nullptr)
{
}


NTStCom::~NTStCom()
{
}

void NTStCom::InitComponent()
{
	Transform = GetComponent<NTTransform>();
}