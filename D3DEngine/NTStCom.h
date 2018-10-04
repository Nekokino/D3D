#pragma once
#include "NTTransform.h"
#include "TimeSystem.h"

class NTStCom : public NTComponent
{
protected:
	Autoptr<NTTransform> Transform;

public:
	virtual void InitComponent() override;
public:
	NTStCom();
	~NTStCom();
};

