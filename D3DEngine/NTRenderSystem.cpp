#include "PreCom.h"
#include "NTRenderSystem.h"
#include "NTRenderer.h"
#include "NTCamera.h"
#include "NTLight.h"
#include "NTWinShortCut.h"
#include "ResourceSystem.h"
#include "NTSampler.h"
#include "DebugFunc.h"
#include "NTMultiRenderTarget.h"


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
	NTWinShortCut::GetMainDevice().ClearTarget();
	NTWinShortCut::GetMainDevice().OMSet();

	ResetSampler();

	CameraMapStartIter = CameraMap.begin();
	CameraMapEndIter = CameraMap.end();

	for (; CameraMapStartIter != CameraMapEndIter; ++CameraMapStartIter)
	{
		bool TmpCheck = false;

		for (size_t i = 0; i < CameraMapStartIter->second->RenderGroup.size(); i++)
		{
			GroupFindIter = RendererMap.find(CameraMapStartIter->second->RenderGroup[i]);

			if (GroupFindIter == RendererMap.end())
			{
				continue;
			}

			NTWinShortCut::GetMainDevice().SetDepthStencilState(L"Basic");

			

			//ListStartIter = GroupFindIter->second.begin();
			//ListEndIter = GroupFindIter->second.end();

			//// 라이트를 모아서 상수버퍼에 세팅

			//LightCheck((*SetStartIter)->RenderGroup[i], SetStartIter);

			//for (; ListStartIter != ListEndIter; ++ListStartIter)
			//{
			//	if ((*ListStartIter)->IsUpdate() == true)
			//	{
			//		(*ListStartIter)->RenderUpdate(); // 렌더러의 래스터라이저 모드로 바꿔주세요.
			//		(*ListStartIter)->TransformUpdate((*SetStartIter));
			//		(*ListStartIter)->Render((*SetStartIter));
			//		(*ListStartIter)->MeshToMatUpdate();
			//		(*ListStartIter)->RenderAfterUpdate(); // 기존 디바이스의 래스터라이저 모드로 바꿔주세요.
			//	}
			//}

			Render_Defferd(CameraMapStartIter->second, GroupFindIter, GroupFindIter->first);
			
			NTWinShortCut::GetMainDevice().SetDepthStencilState(L"LightDepth");

			Render_Defferd_Light(CameraMapStartIter->second, GroupFindIter->first);

			CameraMapStartIter->second->CamTarget->Clear();
			CameraMapStartIter->second->CamTarget->OMSet();
			CameraMapStartIter->second->LightMerge();

			NTWinShortCut::GetMainDevice().SetDepthStencilState(L"Basic");

			Render_Forward(CameraMapStartIter->second, GroupFindIter, GroupFindIter->first);
		}
	}

	NTWinShortCut::GetMainDevice().OMSet();
	MergeScreen();
	

	if (DebugFunc::IsDebug() == true)
	{
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"Always");
		NTWinShortCut::GetMainSceneSystem().GetCurScene()->DbgRender();
		DebugFunc::TargetDebug();
		DebugFunc::RenderLog();
	}

	NTWinShortCut::GetMainDevice().Present();
}

void NTRenderSystem::Render_Forward(Autoptr<NTCamera> _Camera, std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index)
{

	ListStartIter = GroupFindIter->second.begin();
	ListEndIter = GroupFindIter->second.end();

	LightCheck(_Camera, _Index);

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (0 == (*ListStartIter)->RndOpt.IsDefferdOrForward)
		{
			(*ListStartIter)->TransformUpdate(_Camera);
			(*ListStartIter)->TransformConstBufferUpdate();
			(*ListStartIter)->RenderUpdate();
			if (NTDRAWMODE::BASE == (*ListStartIter)->RndOpt.DrawMode)
			{
				for (UINT i = 0; i < (UINT)(*ListStartIter)->GetMeshCount(); i++)
				{
					for (UINT j = 0; j < (UINT)(*ListStartIter)->GetMaterialCount(); j++)
					{
						(*ListStartIter)->Render(_Camera);
						(*ListStartIter)->MaterialTextureNSamplerUpdate(j);
						(*ListStartIter)->MaterialConstBufferUpdate(j);
						(*ListStartIter)->MaterialUpdate(j);
						(*ListStartIter)->MeshUpdate(j);
					}
				}
			}
			else if (NTDRAWMODE::DATA == (*ListStartIter)->RndOpt.DrawMode)
			{
				for (UINT i = 0; i < (UINT)(*ListStartIter)->DrawDataVec.size(); i++)
				{
					(*ListStartIter)->Render(_Camera);
					(*ListStartIter)->MaterialTextureNSamplerUpdate((*ListStartIter)->DrawDataVec[i].Mat);
					(*ListStartIter)->MaterialConstBufferUpdate((*ListStartIter)->DrawDataVec[i].Mat);
					(*ListStartIter)->MaterialUpdate((*ListStartIter)->DrawDataVec[i].Mat);
					(*ListStartIter)->TargetMeshUpdate((*ListStartIter)->DrawDataVec[i].Mesh, (*ListStartIter)->DrawDataVec[i].Vtx, (*ListStartIter)->DrawDataVec[i].Sub);

				}
			}

			(*ListStartIter)->RenderAfterUpdate();
		}
	}
	
}

void NTRenderSystem::Render_Defferd(Autoptr<NTCamera> _Camera, std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index)
{
	Autoptr<NTMultiRenderTarget> DefferdTarget = ResourceSystem<NTMultiRenderTarget>::Find(L"Defferd");
	DefferdTarget->Clear();
	DefferdTarget->OMSet();

	Autoptr<NTMaterial> DefferdMat = ResourceSystem<NTMaterial>::Find(L"DefferdMat");
	Autoptr<NTMaterial> DefferdAniMat = ResourceSystem<NTMaterial>::Find(L"DefferdAniMat");

	if (nullptr == DefferdAniMat)
	{
		tassert(true);
		return;
	}

	if (DefferdMat == nullptr)
	{
		tassert(true);
		return;
	}

	ListStartIter = GroupFindIter->second.begin();
	ListEndIter = GroupFindIter->second.end();

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		if (1 == (*ListStartIter)->RndOpt.IsDefferdOrForward)
		{
			(*ListStartIter)->TransformUpdate(_Camera);
			(*ListStartIter)->TransformConstBufferUpdate();
			(*ListStartIter)->RenderUpdate();
			if (NTDRAWMODE::BASE == (*ListStartIter)->RndOpt.DrawMode)
			{
				for (UINT i = 0; i < (UINT)(*ListStartIter)->GetMeshCount(); i++)
				{
					for (UINT j = 0; j < (UINT)(*ListStartIter)->GetMaterialCount(); j++)
					{
						(*ListStartIter)->Render(_Camera);
						(*ListStartIter)->MaterialTextureNSamplerUpdate(j);
						(*ListStartIter)->MaterialConstBufferUpdate(j);

						if (1 == (*ListStartIter)->RndOpt.IsBoneAni)
						{
							DefferdAniMat->Update();
						}
						else
						{
							DefferdMat->Update();
						}
						(*ListStartIter)->MeshUpdate(i);
					}
				}
			}
			else if (NTDRAWMODE::DATA == (*ListStartIter)->RndOpt.DrawMode)
			{
				for (UINT i = 0; i < (UINT)(*ListStartIter)->DrawDataVec.size(); i++)
				{
					DrawData Data = (*ListStartIter)->DrawDataVec[i];
					(*ListStartIter)->Render(_Camera);
					(*ListStartIter)->MaterialTextureNSamplerUpdate((*ListStartIter)->DrawDataVec[i].Mat);
					(*ListStartIter)->MaterialConstBufferUpdate((*ListStartIter)->DrawDataVec[i].Mat);

					if (1 == (*ListStartIter)->RndOpt.IsBoneAni)
					{
						DefferdAniMat->Update();
					}
					else
					{
						DefferdMat->Update();
					}

					(*ListStartIter)->TargetMeshUpdate((*ListStartIter)->DrawDataVec[i].Mesh, (*ListStartIter)->DrawDataVec[i].Vtx, (*ListStartIter)->DrawDataVec[i].Sub);
				}
			}
			(*ListStartIter)->RenderAfterUpdate();
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
	CameraMap.insert(std::map<int, Autoptr<NTCamera>>::value_type(_Cam->GetOrder(), _Cam));
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

void NTRenderSystem::LightCheck(Autoptr<NTCamera> _Camera, int _Group)
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
			Autoptr<NTLight> Light = (*LightStartIter);
			Light->CalLightData(_Camera);
			Data.ArrLight[Count] = (*LightStartIter)->Data;
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

	return;
}

void NTRenderSystem::Render_Defferd_Light(Autoptr<NTCamera> _Cam, int _Group)
{
	Autoptr<NTMultiRenderTarget> LightTarget = ResourceSystem<NTMultiRenderTarget>::Find(L"Light");
	LightTarget->Clear();
	LightTarget->OMSet();

	LightStartIter = LightSet.begin();
	LightEndIter = LightSet.end();

	for (; LightStartIter != LightEndIter; ++LightStartIter)
	{
		if (true == (*LightStartIter)->IsLight(_Group))
		{
			Autoptr<NTLight> Light = *LightStartIter;
			Light->CalLightData(_Cam);
			Light->LightRender(_Cam);
			NTWinShortCut::GetMainDevice().SetDepthStencilState(L"LightDepth");
		}
	}
}

void NTRenderSystem::MergeScreen()
{
	CameraMapStartIter = CameraMap.begin();
	CameraMapEndIter = CameraMap.end();

	for (; CameraMapStartIter != CameraMapEndIter; ++CameraMapStartIter)
	{
		CameraMapStartIter->second->ScreenMerge();
	}
}