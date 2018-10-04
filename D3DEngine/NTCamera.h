#pragma once

#include "NTStCom.h"

#include <set>

class NTTransform;
class NTRenderSystem;
class NTCamera final : public NTStCom
{
public:
	friend NTRenderSystem;

public:
	enum PROJMODE
	{
		CPM_PERSPECTIVE,
		CPM_ORTHOGRAPHIC,
	};

	enum SIZEMODE
	{
		CSM_WINDOW,
		CSM_CUSTOM,
	};

private:
	NTMAT View;
	NTMAT Projection;
	NTMAT VP;
	PROJMODE PMode;
	SIZEMODE SMode;
	NTVEC ScreenSize;
	float Fov;
	float Near;
	float Far;
private:
	std::set<int> RenderGroup;

public:
	const NTMAT& GetVP() const
	{
		return VP;
	}

public:
	bool Init() override;
	void SetSMode(SIZEMODE _SMode)
	{
		SMode = _SMode;
	}
	void SetCustomSize(NTVEC2 _Size)
	{
		if (SMode != CSM_CUSTOM)
		{
			return;
		}

		ScreenSize = _Size;
	}

	const NTVEC& GetScreenSize()
	{
		return ScreenSize;
	}

public:
	template<typename ...Rest>
	void PushRenderLayer(Rest... Arg)
	{
		PushLayer(Arg...);
	}

private:
	template<typename ...Rest>
	void PushLayer(int _Data, Rest... Arg)
	{
		RenderGroup.insert(_Data);
		PushLayer(Arg...);
	}

	void PushLayer() {}

private:
	virtual void MainUpdate() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;
public:
	NTCamera();
	~NTCamera();
};

