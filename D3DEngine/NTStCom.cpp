#include "PreCom.h"
#include "NTStCom.h"


NTStCom::NTStCom()
{
}


NTStCom::~NTStCom()
{
}

void NTStCom::InitComponent()
{
	Transform = GetComponent<NTTransform>();
}