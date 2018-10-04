#pragma once

class NTObject;
class NTComponent;
class NTObjectParent
{
public:
	friend NTObject;
	friend NTComponent;
private:
	NTObject* Obj;

public:
	NTObject* GetNTObject()
	{
		return Obj;
	}

private:
	void SetObject(NTObject* _Obj)
	{
		Obj = _Obj;
	}

protected:
	NTObjectParent() : Obj(nullptr) {}
	~NTObjectParent() {}
};