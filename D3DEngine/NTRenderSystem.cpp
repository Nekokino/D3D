#include "PreCom.h"
#include "NTRenderSystem.h"
#include "NTRenderer.h"
#include "NTCamera.h"
#include "NTLight.h"
#include "NTWinShortCut.h"
#include "ResourceSystem.h"
#include "NTSampler.h"


NTRenderSystem::NTRenderSystem()
{
}


NTRenderSystem::~NTRenderSystem()
{
}

void NTRenderSystem::ResetSampler()
{
	Autoptr<NTSampler> Sampler = ResourceSystem<NTSampler>::Find(L"DefaultSampler");

	if (nullptr == Sampler)
	{
		tassert(true);
		return;
	}

	Sampler->Update(0);
	Sampler->Update(1);
	Sampler->Update(2);
	Sampler->Update(3);
	Sampler->Update(4);
	Sampler->Update(5);
	Sampler->Update(6);
	Sampler->Update(7);
}

void NTRenderSystem::Render()
{
	ResetSampler();

	SetStartIter = CameraSet.begin();
	SetEndIter = CameraSet.end();

	for (; SetStartIter != SetEndIter; ++SetStartIter)
	{

		bool TmpCheck = false;

		for (size_t i = 0; i < (*SetStartIter)->RenderGroup.size(); i++)
		{
			GroupFindIter = RendererMap.find((*SetStartIter)->RenderGroup[i]);

			if (GroupFindIter == RendererMap.end())
			{
				continue;
			}

			ListStartIter = GroupFindIter->second.begin();
			ListEndIter = GroupFindIter->second.end();

			// 라이트를 모아서 상수버퍼에 세팅

			LightCheck((*SetStartIter)->RenderGroup[i], SetStartIter);

			for (; ListStartIter != ListEndIter; ++ListStartIter)
			{
				if ((*ListStartIter)->IsUpdate() == true)
				{
					(*ListStartIter)->RenderUpdate(); // 렌더러의 래스터라이저 모드로 바꿔주세요.
					(*ListStartIter)->TransformUpdate((*SetStartIter));
					(*ListStartIter)->Render((*SetStartIter));
					(*ListStartIter)->MeshToMatUpdate();
					(*ListStartIter)->RenderAfterUpdate(); // 기존 디바이스의 래스터라이저 모드로 바꿔주세요.
				}
			}
		}
	}
}

// z순서대로
bool NTRenderSystem::ZOrderSort(Autoptr<NTRenderer> _Left, Autoptr<NTRenderer> _Right)
{
	if (_Left->GetWorldSubPivot().z > _Right->GetWorldSubPivot().z)
	{
		return true;
	}

	return false;
}

void NTRenderSystem::PushCamera(NTCamera* _Cam)
{
	tassert(nullptr == _Cam);
	CameraSet.insert(_Cam);
}

void NTRenderSystem::PushRenderer(NTRenderer* _Renderer)
{
	tassert(nullptr == _Renderer);

	GroupFindIter = RendererMap.find(_Renderer->Order);

	if (GroupFindIter == RendererMap.end())
	{
		RendererMap.insert(std::map<int, std::list<Autoptr<NTRenderer>>>::value_type(_Renderer->Order, std::list<Autoptr<NTRenderer>>()));
		GroupFindIter = RendererMap.find(_Renderer->Order);
	}

	GroupFindIter->second.push_back(_Renderer);
}

void NTRenderSystem::PushOverRenderer(Autoptr<NTRenderer> _Renderer)
{
	tassert(nullptr == _Renderer);

	GroupFindIter = RendererMap.find(_Renderer->GetOrder());

	if (GroupFindIter == RendererMap.end())
	{
		RendererMap.insert(std::map<int, std::list<Autoptr<NTRenderer>>>::value_type(_Renderer->GetOrder(), std::list<Autoptr<NTRenderer>>()));
		GroupFindIter = RendererMap.find(_Renderer->GetOrder());
	}

	GroupFindIter->second.push_back(_Renderer);
}

void NTRenderSystem::Release()
{
	MapStartIter = RendererMap.begin();
	MapEndIter = RendererMap.end();

	for (; MapStartIter != MapEndIter; ++MapStartIter)
	{
		ListStartIter = MapStartIter->second.begin();
		ListEndIter = MapStartIter->second.end();

		for (; ListStartIter != ListEndIter;)
		{
			if (true == (*ListStartIter)->IsDeath())
			{
				ListStartIter = MapStartIter->second.erase(ListStartIter);
			}
			else
			{
				++ListStartIter;
			}
		}
	}
}

Autoptr<NTRenderer> NTRenderSystem::GetRenderer(const wchar_t* _Name, int _Order)
{
	std::map<int, std::list<Autoptr<NTRenderer>>>::iterator FindIter = RendererMap.find(_Order);

	if (FindIter == RendererMap.end())
	{
		return nullptr;
	}

	std::list<Autoptr<NTRenderer>>::iterator StartIter = FindIter->second.begin();
	std::list<Autoptr<NTRenderer>>::iterator EndIter = FindIter->second.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (0 == lstrcmpW(_Name, (*StartIter)->GetName()))
		{
			return (*StartIter);
		}
	}

	return nullptr;
}

void NTRenderSystem::PushLight(NTLight* _Light)
{
	LightSet.insert(_Light);
}

void NTRenderSystem::LightCheck(int _Group, const std::set<Autoptr<NTCamera>>::iterator& _CamIter)
{
	LightStartIter = LightSet.begin();
	LightEndIter = LightSet.end();

	int Count = 0;
	NTLight::LightCBData Data;
	memset(&Data, 0, sizeof(NTLight::LightCBData));

	for (; LightStartIter != LightEndIter; ++LightStartIter)
	{
		if (true == (*LightStartIter)->IsLight(_Group))
		{
			Data.ArrLight[Count] = (*LightStartIter)->Data;
			Data.ArrLight[Count].Dir = -(*_CamIter)->GetView().MulZero(Data.ArrLight[Count].Dir);
			Data.ArrLight[Count].Pos = -(*_CamIter)->GetView().MulOne(Data.ArrLight[Count].Pos);
			++Count;
			if (10 <= Count)
			{
				break;
			}
		}
	}

	Data.LightCount = Count;

	NTWinShortCut::GetMainDevice().SetConstBuffer<NTLight::LightCBData>(L"LightData", Data, STYPE::ST_VS);
	NTWinShortCut::GetMainDevice().SetConstBuffer<NTLight::LightCBData>(L"LightData", Data, STYPE::ST_PX);
}