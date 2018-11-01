#include "PreCom.h"
#include "NTRenderSystem.h"
#include "NTRenderer.h"
#include "NTCamera.h"


NTRenderSystem::NTRenderSystem()
{
}


NTRenderSystem::~NTRenderSystem()
{
}

void NTRenderSystem::Render()
{
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

			for (; ListStartIter != ListEndIter; ++ListStartIter)
			{
				if ((*ListStartIter)->IsUpdate() == true)
				{
					(*ListStartIter)->RenderUpdate(); // 렌더러의 래스터라이저 모드로 바꿔주세요.
					(*ListStartIter)->Render((*SetStartIter));
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

void NTRenderSystem::PushRenderer(NTRenderer* _Renderer, int _Order)
{
	tassert(nullptr == _Renderer);

	GroupFindIter = RendererMap.find(_Order);

	if (GroupFindIter == RendererMap.end())
	{
		RendererMap.insert(std::map<int, std::list<Autoptr<NTRenderer>>>::value_type(_Order, std::list<Autoptr<NTRenderer>>()));
		GroupFindIter = RendererMap.find(_Order);
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