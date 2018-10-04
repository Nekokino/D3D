#pragma once
#include "NTNameTag.h"
#include "NTTypeParent.h"
#include "RefNAutoptr.h"
class NTSwitcher : public NTNameTag, public NTTypeParent, public RefCounter
{
private:
	bool bUpdate;
	bool bDeath;

public:
	virtual bool IsUpdate()
	{
		return (false == bDeath) && (true == bUpdate);
	}

	virtual bool IsDeath()
	{
		return bDeath;
	}

	virtual void SetUpdate(bool _Value)
	{
		bUpdate = _Value;
	}

	virtual void Death()
	{
		bDeath = true;
	}

public:
	virtual bool Init();
	virtual void PreUpdate();
	virtual void MainUpdate();
	virtual void AfterUpdate();
	virtual void DbgUpdate();
	virtual void DbgRender();

protected:
	NTSwitcher();
	~NTSwitcher();
};

