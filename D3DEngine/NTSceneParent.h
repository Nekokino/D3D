#pragma once


class NTScene;
class NTSceneParent
{
public:
	friend class NTScene;
	friend class NTObject;
	friend class NTComponent;

private:
	NTScene* Scene;

public:
	NTScene* GetScene()
	{
		return Scene;
	}

private:
	void SetScene(NTScene* _Scene)
	{
		Scene = _Scene;
	}
protected:
	NTSceneParent() : Scene(nullptr) {}
	~NTSceneParent() {}
};

