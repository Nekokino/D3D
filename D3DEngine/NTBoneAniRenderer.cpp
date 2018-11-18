#include "PreCom.h"
#include "NTBoneAniRenderer.h"
#include "TimeSystem.h"
#include "NTTexture.h"
#include "ResourceSystem.h"
#include "DirectValueHeader.h"

NTBoneAniRenderer::NTBoneAniRenderer() : UpdateTime(0.0f), FrameCount(30), CurTime(0.0f), ClipIndex(0)
{
	RndOpt.DrawMode = NTDRAWMODE::DATA;
	RndOpt.IsBoneAni = 1;
}


NTBoneAniRenderer::~NTBoneAniRenderer()
{
	if (nullptr != Loader)
	{
		delete Loader;
	}
}

void NTBoneAniRenderer::Test(const wchar_t * _Path)
{
	Loader = new NTFbxLoader();

	Loader->LoadFbx(_Path);

	NTFBX* Fbx = Loader->NewFbx;

	for (size_t MeshIdx = 0; MeshIdx < Fbx->MeshDataVec.size() ; MeshIdx++)
	{
		NTFbxMeshData* MeshData = Fbx->MeshDataVec[MeshIdx];

		if (nullptr == MeshData)
		{
			tassert(true);
			return;
		}

		Autoptr<NTMesh> NewMesh = new NTMesh();
		NewMesh->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (false == NewMesh->CreateVertex((UINT)MeshData->VtxVec.size(), sizeof(NTFbxVtxData), D3D11_USAGE_DYNAMIC, &MeshData->VtxVec[0]))
		{
			tassert(true);
			return;
		}

		for (size_t SubIdx = 0; SubIdx < MeshData->IdxVec.size(); SubIdx++)
		{
			NewMesh->CreateIndex((UINT)MeshData->IdxVec[SubIdx].size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::GetFormat(), &MeshData->IdxVec[SubIdx][0]);
		}

		SetMesh(NewMesh, (int)MeshIdx);

		for (UINT MatIdx = 0; MatIdx < (UINT)MeshData->MatDataVec.size(); MatIdx++)
		{
			switch (RndOpt.IsDefferdOrForward)
			{
			case 0:
				SetMaterial(L"Mesh3DMat", MatIdx);
			case 1:
				SetMaterial(L"DefferdAniMat", MatIdx);
			default:
				break;
			}

			Autoptr<NTMaterial> CloneMat = GetMaterial(MatIdx);

			NTFbxMatData* Data = MeshData->MatDataVec[MatIdx];

			if (Data->Diffuse != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Diffuse.c_str());
				CloneMat->AddTextureData(TEXTYPE::TT_COLOR, 0, Tex->GetFullFileName());
			}

			if (Data->Bump != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Bump.c_str());
				CloneMat->AddTextureData(TEXTYPE::TT_BUMP, 1, Tex->GetFullFileName());
			}

			if (Data->Specular != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Specular.c_str());
				CloneMat->AddTextureData(TEXTYPE::TT_SPEC, 2, Tex->GetFullFileName());
			}

			AddDrawData((UINT)MeshIdx, 0, MatIdx, MatIdx);
		}
	}

	if (0 >= Loader->NewFbx->AniVec.size())
	{
		return;
	}

	BoneTex = new NTTexture();
	BoneTex->Create(Loader->BoneCount * 4, 1, D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC);

	CurAniMatData.resize(Loader->BoneCount);
}

void NTBoneAniRenderer::EndUpdate()
{
	if (0 >= Loader->NewFbx->AniVec.size())
	{
		return;
	}

	CurTime = 0.0f;

	NTFBX* Fbx = Loader->NewFbx;

	UpdateTime += TimeSystem::DeltaTime();

	if (UpdateTime >= Fbx->AniVec[ClipIndex]->TimeLength)
	{
		UpdateTime = 0.0f;
	}

	CurTime = (float)(Fbx->AniVec[ClipIndex]->StartTime.GetSecondDouble() + UpdateTime);

	int FrameIndex = (int)(CurTime * FrameCount);
	int NextFrameIndex = 0;

	if (FrameIndex >= Fbx->AniVec[ClipIndex]->TimeLength - 1)
	{
		UpdateTime = 0.0f;
		FrameIndex = 0;
	}

	NextFrameIndex = FrameIndex + 1;

	for (size_t i = 0; i < Loader->BoneCount; i++)
	{
		if (Fbx->BoneVec[i]->KeyFrameVec.empty())
		{
			CurAniMatData[i] = NTFbxLoader::FbxMatConvert(Fbx->BoneVec[i]->BoneMat);
			continue;
		}

		NTKeyFrame& CurFrame = Fbx->BoneVec[i]->KeyFrameVec[FrameIndex];
		NTKeyFrame& NextFrame = Fbx->BoneVec[i]->KeyFrameVec[NextFrameIndex];

		float FrameTime = (float)CurFrame.Time;
		float NextFrameTime = (float)NextFrame.Time;

		float Percent = (CurTime - FrameTime) / (1.0f / FrameCount);

		DirectX::XMVECTOR SC = NTFbxLoader::FbxVec4Convert(CurFrame.FrameMat.GetS());
		DirectX::XMVECTOR SN = NTFbxLoader::FbxVec4Convert(NextFrame.FrameMat.GetS());

		DirectX::XMVECTOR TC = NTFbxLoader::FbxVec4Convert(CurFrame.FrameMat.GetT());
		DirectX::XMVECTOR TN = NTFbxLoader::FbxVec4Convert(NextFrame.FrameMat.GetT());

		DirectX::XMVECTOR QC = NTFbxLoader::FbxQuaternionConvert(CurFrame.FrameMat.GetQ());
		DirectX::XMVECTOR QN = NTFbxLoader::FbxQuaternionConvert(NextFrame.FrameMat.GetQ());

		DirectX::XMVECTOR S = DirectX::XMVectorLerp(SC, SN, Percent);
		DirectX::XMVECTOR T = DirectX::XMVectorLerp(TC, TN, Percent);
		DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(QC, QN, Percent);

		DirectX::XMVECTOR Zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		NTMAT OffsetMat = NTFbxLoader::FbxMatConvert(Fbx->BoneVec[i]->OffsetMat);

		CurAniMatData[i] = OffsetMat * DirectX::XMMatrixAffineTransformation(S, Zero, Q, T);
	}

	BoneTex->SetPixel(&CurAniMatData[0], sizeof(NTMAT) * CurAniMatData.size());
}

void NTBoneAniRenderer::Render(Autoptr<NTCamera> _Camera)
{
	if (nullptr != BoneTex)
	{
		BoneTex->Update(10);
	}
}