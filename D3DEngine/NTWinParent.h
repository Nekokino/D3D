#pragma once

class NTWindow;
class NTWinParent
{
public:
	friend class NTSceneSystem;
	friend class NTObject;
	friend class NTScene;

private:
	NTWindow* Win;

public:
	NTWindow* GetNTWindow()
	{
		return Win;
	}

private:
	void SetNTWindow(NTWindow* _Win)
	{
		Win = _Win;
	}
protected:
	NTWinParent(NTWindow* _Win) : Win(_Win) {}
	NTWinParent() : Win(nullptr) {}
	~NTWinParent() {}
};

