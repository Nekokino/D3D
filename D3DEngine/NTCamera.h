#pragma once

#include "NTStCom.h"
#include <vector>
#include "NTMultiRenderTarget.h"

class NTMesh;
class NTMaterial;
class NTTransform;
class NTRenderSystem;
class NTCamera final : public NTStCom
{
public:
	friend NTRenderSystem;
	friend class DebugFunc;

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
	std::vector<int> RenderGroup;

public:
	const NTMAT& GetView() const
	{
		return View;
	}

	const NTMAT& GetProjection() const
	{
		return Projection;
	}

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

	void SetPMode(PROJMODE _Mode)
	{
		PMode = _Mode;
	}

	void ChangePMode()
	{
		if (PMode == PROJMODE::CPM_ORTHOGRAPHIC)
		{
			PMode = PROJMODE::CPM_PERSPECTIVE;
		}
		else
		{
			PMode = PROJMODE::CPM_ORTHOGRAPHIC;
		}
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

	void SetNear(float _Near)
	{
		Near = _Near;
	}

	void SetFar(float _Far)
	{
		Far = _Far;
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
		RenderGroup.push_back(_Data);
		PushLayer(Arg...);
	}

	void PushLayer() {}

private:
	virtual void MainUpdate() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;

private:
	Autoptr<NTMesh> CamMesh;
	Autoptr<NTMaterial> CamMaterial;

private:
	void MergeRender();

private:
	Autoptr<NTMultiRenderTarget> CamTarget;
public:
	NTCamera();
	~NTCamera();
};

