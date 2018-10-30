#pragma once
#include "NTTransform.h"
#include "TimeSystem.h"

class NTStCom : public NTComponent
{
protected:
	NTTransform* Transform;

public:
	NTTransform* GetTransform()
	{
		return Transform;
	}

public:
	virtual void InitComponent() override;
public:
	NTStCom();
	~NTStCom();
};

