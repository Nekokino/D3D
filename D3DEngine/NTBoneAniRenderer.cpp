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
}

void NTBoneAniRenderer::Test(const wchar_t * _Path)
{
	FbxMesh = ResourceSystem<NTFbxData>::Load(_Path);
	InitMesh();
}

void NTBoneAniRenderer::SetFbx(const wchar_t * _Name)
{
	FbxMesh = ResourceSystem<NTFbxData>::Find(_Name);
	InitMesh();
}

NTMAT NTBoneAniRenderer::GetBoneMat(const wchar_t * _Name)
{
	NTFbxBiBoneData* Bone = FbxMesh->FindBone(_Name);

	return CurAniBoneData[Bone->Index];
}

NTMAT NTBoneAniRenderer::GetWorldBoneMat(const wchar_t * _Name)
{
	return GetBoneMat(_Name) * GetTransform()->GetWorldMatrix();
}

void NTBoneAniRenderer::EndUpdate()
{
	if (FbxMesh == nullptr)
	{
		return;
	}

	if (0 >= FbxMesh->Data.AniVec.size())
	{
		return;
	}

	CurTime = 0.0f;

	UpdateTime += TimeSystem::DeltaTime();

	if (UpdateTime >= FbxMesh->Data.AniVec[ClipIndex].TimeLength)
	{
		UpdateTime = 0.0f;
	}

	CurTime = (float)(FbxMesh->Data.AniVec[ClipIndex].StartTime.GetSecondDouble() + UpdateTime);

	int FrameIndex = (int)(CurTime * FrameCount);
	int NextFrameIndex = 0;

	if (FrameIndex >= FbxMesh->Data.AniVec[ClipIndex].TimeLength - 1)
	{
		UpdateTime = 0.0f;
		FrameIndex = 0;
	}

	NextFrameIndex = FrameIndex + 1;

	for (size_t i = 0; i < FbxMesh->Data.BoneVec.size(); i++)
	{
		if (FbxMesh->Data.BoneVec[i]->KeyFrameVec.empty())
		{
			CurAniMatData[i] = FbxMesh->Data.BoneVec[i]->BoneMat;
			continue;
		}

		NTKeyFrame& CurFrame = FbxMesh->Data.BoneVec[i]->KeyFrameVec[FrameIndex];
		NTKeyFrame& NextFrame = FbxMesh->Data.BoneVec[i]->KeyFrameVec[NextFrameIndex];

		float FrameTime = (float)CurFrame.Time;
		float NextFrameTime = (float)NextFrame.Time;

		float Percent = (CurTime - FrameTime) / (1.0f / FrameCount);

		DirectX::XMVECTOR S = DirectX::XMVectorLerp(CurFrame.S, NextFrame.S, Percent);
		DirectX::XMVECTOR T = DirectX::XMVectorLerp(CurFrame.T, NextFrame.T, Percent);
		DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(CurFrame.Q, NextFrame.Q, Percent);

		DirectX::XMVECTOR Zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		CurAniBoneData[i] = DirectX::XMMatrixAffineTransformation(S, Zero, Q, T);
		CurAniMatData[i] = FbxMesh->Data.BoneVec[i]->OffsetMat * CurAniBoneData[i];
	}

	BoneTex->SetPixel(&CurAniMatData[0], sizeof(NTMAT) * CurAniMatData.size());
}

void NTBoneAniRenderer::InitMesh()
{
	for (size_t MeshIdx = 0; MeshIdx < FbxMesh->Data.MeshVec.size(); MeshIdx++)
	{
		NTFbxBiMeshData* MeshData = FbxMesh->Data.MeshVec[MeshIdx];

		if (nullptr == MeshData)
		{
			tassert(true);
			return;
		}

		Autoptr<NTMesh> NewMesh = new NTMesh();
		NewMesh->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (false == NewMesh->CreateVertex(MeshData->VtxCount, MeshData->VtxSize, MeshData->Usage, MeshData->BDVtx))
		{
			tassert(true);
			return;
		}

		for (size_t SubIdx = 0; SubIdx < MeshData->IdxVec.size(); SubIdx++)
		{
			NTFbxBiIndexData* Data = &MeshData->IdxVec[SubIdx];

			NewMesh->CreateIndex(Data->IdxCount, Data->IdxSize, Data->Usage, Data->Format, Data->BDIdx);
		}

		SetMesh(NewMesh, (int)MeshIdx);

		for (UINT MatIdx = 0; MatIdx < (UINT)MeshData->MatVec.size(); MatIdx++)
		{
			switch (RndOpt.IsDefferdOrForward)
			{
			case 0:
				SetMaterial(L"Mesh3DMat", MatIdx);
			case 1:
				if (0 != FbxMesh->Data.AniVec.size())
				{
					SetMaterial(L"DefferdAniMat", MatIdx);
				}
				else
				{
					SetMaterial(L"DefferdMat", MatIdx);
				}
			default:
				break;
			}

			Autoptr<NTMaterial> CloneMat = GetMaterial(MatIdx);

			NTFbxMatData* Data = &MeshData->MatVec[MatIdx];

			if (Data->Diffuse != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Diffuse);
				CloneMat->AddTextureData(TEXTYPE::TT_COLOR, 0, Tex->GetFullFileName());
			}

			if (Data->Bump != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Bump);
				CloneMat->AddTextureData(TEXTYPE::TT_BUMP, 1, Tex->GetFullFileName());
			}

			if (Data->Specular != L"")
			{
				Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Load(Data->Specular);
				CloneMat->AddTextureData(TEXTYPE::TT_SPEC, 2, Tex->GetFullFileName());
			}

			AddDrawData((UINT)MeshIdx, 0, MatIdx, MatIdx);
		}
	}

	if (0 >= FbxMesh->Data.AniVec.size())
	{
		RndOpt.IsBoneAni = 0;
		return;
	}

	BoneTex = new NTTexture();
	BoneTex->Create((UINT)FbxMesh->Data.BoneVec.size() * 4, 1, D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC);

	CurAniMatData.resize(FbxMesh->Data.BoneVec.size());
	CurAniBoneData.resize(FbxMesh->Data.BoneVec.size());
}

void NTBoneAniRenderer::Render(Autoptr<NTCamera> _Camera)
{
	if (nullptr != BoneTex)
	{
		BoneTex->Update(10);
	}
}