#include "NTFBX.h"
#include "PreCom.h"
#include "DirectValueHeader.h"
#include <atlstr.h>

NTFBXLoader::NTFBXLoader() : Manager(nullptr), Scene(nullptr), NewFbx(nullptr), BoneCount(0)
{
}

NTFBXLoader::~NTFBXLoader()
{
}

void NTFBXLoader::Load(const wchar_t * _Path)
{
	NewFbx = new NTFBX();

	// Fbx로딩을 위한 포인터
	Manager = FbxManager::Create();

	// OS와 시스템에 맞는 버전 세팅
	FbxIOSettings* SetP = FbxIOSettings::Create(Manager, IOSROOT);
	Manager->SetIOSettings(SetP);

	// 메쉬 정보 기본 클래스
	Scene = FbxScene::Create(Manager, "");

	// 로드하기 위한 객체
	FbxImporter* Importer = FbxImporter::Create(Manager, "");

	// 문자 인코딩
	Importer->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, Manager->GetIOSettings());
	Importer->Import(Scene);

	if (Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::eMax)
	{
		Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMax);
	}

	CalBoneCount(Scene->GetRootNode());

	NewFbx->BoneVec.reserve(BoneCount);

	// 삼각화
	Triangulate(Scene->GetRootNode());

	LoadBone(Scene->GetRootNode(), 0, nullptr);
	AniCheck();

	FbxMeshData(Scene->GetRootNode());
	

	Importer->Destroy();
	Scene->Destroy();
	SetP->Destroy();
	Manager->Destroy();

	delete NewFbx;
}

void NTFBXLoader::CalBoneCount(FbxNode * _Node)
{
	++BoneCount;
	int Count = _Node->GetChildCount();
	for (int i = 0; i < Count; i++)
	{
		CalBoneCount(_Node->GetChild(i));
	}
}

void NTFBXLoader::LoadBone(FbxNode* _Node, unsigned int _Depth, NTBone * _Parent)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	NTBone* NewBone = nullptr;

	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		NewBone = new NTBone();
		NewBone->Name = CA2W(_Node->GetName()).m_psz;
		NewBone->Depth = _Depth++;
		NewBone->Index = (int)NewFbx->BoneVec.size();
		NewBone->Parent = _Parent;
		NewFbx->BoneVec.push_back(NewBone);
		NewFbx->BoneMap.insert(std::multimap<std::wstring, NTBone*>::value_type(NewBone->Name, NewBone));
	}
	else
	{
		++_Depth;
	}

	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		LoadBone(_Node->GetChild(i), _Depth, NewBone);
	}
}

void NTFBXLoader::AniCheck()
{
	Scene->FillAnimStackNameArray(NewFbx->AniNameArray);

	for (int i = 0; i < NewFbx->AniNameArray.GetCount(); i++)
	{
		FbxAnimStack* AniStack = Scene->FindMember<FbxAnimStack>(NewFbx->AniNameArray[i]->Buffer());

		if (nullptr == AniStack)
		{
			continue;
		}

		NTFbxAniInfo* NewInfo = new NTFbxAniInfo();

		NewInfo->AniName = CA2W(AniStack->GetName());

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AniStack->GetName());
		NewInfo->StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		NewInfo->EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		NewInfo->Mode = Scene->GetGlobalSettings().GetTimeMode();
		NewInfo->TimeLength = NewInfo->EndTime.GetFrameCount(NewInfo->Mode) - NewInfo->StartTime.GetFrameCount(NewInfo->Mode);

		NewFbx->AniMap.insert(std::map<std::wstring, NTFbxAniInfo*>::value_type(NewInfo->AniName, NewInfo));
	}
}

void NTFBXLoader::Triangulate(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (nullptr == Attr)
	{
		tassert(true);
		return;
	}

	if (Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxGeometryConverter Con(Manager);
		Con.Triangulate(Attr, true);
	}

	int ChildCount = _Node->GetChildCount();
	for (int i = 0; i < ChildCount; i++)
	{
		Triangulate(_Node->GetChild(i));
	}
}

void NTFBXLoader::FbxMeshData(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* NewMesh = _Node->GetMesh();

		if (nullptr != NewMesh)
		{
			NTFbxMeshData* NewMeshData = new NTFbxMeshData();
			NewFbx->MeshDataVec.push_back(NewMeshData);

			NewMeshData->Name = CA2W(NewMesh->GetName(), CP_UTF8);

			int VtxCount = NewMesh->GetControlPointsCount();
			NewMeshData->SetVtxCount(VtxCount);

			FbxVector4* FbxPos = NewMesh->GetControlPoints();

			for (int i = 0; i < VtxCount; i++)
			{
				NewMeshData->PosVec[i].x = (float)FbxPos[i].mData[0];
				NewMeshData->PosVec[i].y = (float)FbxPos[i].mData[2];
				NewMeshData->PosVec[i].z = (float)FbxPos[i].mData[1];
				NewMeshData->PosVec[i].w = 1.0f;
			}

			int TriCount = NewMesh->GetPolygonCount();

			int MtrCount = NewMesh->GetNode()->GetMaterialCount();

			NewMeshData->IdxVec.resize(MtrCount);

			FbxGeometryElementMaterial* Mtrl = NewMesh->GetElementMaterial();

			int IdxSize = NewMesh->GetPolygonSize(0);

			if (3 != IdxSize)
			{
				tassert(true);
				return;
			}

			IDX32 Tmp = {};
			UINT CurVtx = 0;

			for (int i = 0; i < TriCount; i++)
			{
				for (int j = 0; j < IdxSize; j++)
				{
					Tmp.p[j] = NewMesh->GetPolygonVertex(i, j);
					FbxNormal(NewMesh, NewMeshData, Tmp.p[j], CurVtx);
					FbxTangent(NewMesh, NewMeshData, Tmp.p[j], CurVtx);
					FbxBiNormal(NewMesh, NewMeshData, Tmp.p[j], CurVtx);
					FbxUv(NewMesh, NewMeshData, Tmp.p[j], CurVtx);
					++CurVtx;
				}

				UINT SubIdx = Mtrl->GetIndexArray().GetAt(i);
				NewMeshData->IdxVec[SubIdx].push_back(Tmp.p[0]);
				NewMeshData->IdxVec[SubIdx].push_back(Tmp.p[2]);
				NewMeshData->IdxVec[SubIdx].push_back(Tmp.p[1]);
			}

			if (false == NewFbx->AniMap.empty())
			{
				FbxAniData(NewMesh, NewMeshData);
			}
			else
			{
				NewMeshData->bAnimated = false;
			}
		}
	}
}

void NTFBXLoader::FbxNormal(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementNormalCount();

	if (1 != Count)
	{
		tassert(true);
		return;
	}

	FbxGeometryElementNormal* Normal = _Mesh->GetElementNormal();
	UINT Idx = 0;

	if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (Normal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Vtx;
		}
		else
		{
			Idx = Normal->GetIndexArray().GetAt(_Vtx);
		}
	}
	else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (Normal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Idx;
		}
		else
		{
			Idx = Normal->GetIndexArray().GetAt(_Idx);
		}
	}

	FbxVector4 Data = Normal->GetDirectArray().GetAt(Idx);

	_MeshData->NormalVec[_Idx].x = (float)Data.mData[0];
	_MeshData->NormalVec[_Idx].y = (float)Data.mData[2];
	_MeshData->NormalVec[_Idx].z = (float)Data.mData[1];
	_MeshData->NormalVec[_Idx].w = 1.0f;

}

void NTFBXLoader::FbxTangent(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementTangentCount();

	if (1 != Count)
	{
		tassert(true);
		return;
	}

	FbxGeometryElementTangent* Tangent = _Mesh->GetElementTangent();
	UINT Idx = 0;

	if (Tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (Tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Vtx;
		}
		else
		{
			Idx = Tangent->GetIndexArray().GetAt(_Vtx);
		}
	}
	else if (Tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (Tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Idx;
		}
		else
		{
			Idx = Tangent->GetIndexArray().GetAt(_Idx);
		}
	}

	FbxVector4 Data = Tangent->GetDirectArray().GetAt(Idx);

	_MeshData->TangentVec[_Idx].x = (float)Data.mData[0];
	_MeshData->TangentVec[_Idx].y = (float)Data.mData[2];
	_MeshData->TangentVec[_Idx].z = (float)Data.mData[1];
	_MeshData->TangentVec[_Idx].w = 1.0f;
}

void NTFBXLoader::FbxBiNormal(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementBinormalCount();

	if (1 != Count)
	{
		tassert(true);
		return;
	}

	FbxGeometryElementBinormal* BiNormal = _Mesh->GetElementBinormal();
	UINT Idx = 0;

	if (BiNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (BiNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Vtx;
		}
		else
		{
			Idx = BiNormal->GetIndexArray().GetAt(_Vtx);
		}
	}
	else if (BiNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (BiNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Idx;
		}
		else
		{
			Idx = BiNormal->GetIndexArray().GetAt(_Idx);
		}
	}

	FbxVector4 Data = BiNormal->GetDirectArray().GetAt(Idx);

	_MeshData->BiNormalVec[_Idx].x = (float)Data.mData[0];
	_MeshData->BiNormalVec[_Idx].y = (float)Data.mData[2];
	_MeshData->BiNormalVec[_Idx].z = (float)Data.mData[1];
	_MeshData->BiNormalVec[_Idx].w = 1.0f;
}

void NTFBXLoader::FbxUv(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementUVCount();

	if (1 != Count)
	{
		tassert(true);
		return;
	}

	FbxGeometryElementUV* Uv = _Mesh->GetElementUV();
	UINT Idx = 0;

	if (Uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (Uv->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Vtx;
		}
		else
		{
			Idx = Uv->GetIndexArray().GetAt(_Vtx);
		}
	}
	else if (Uv->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (Uv->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _Idx;
		}
		else
		{
			Idx = Uv->GetIndexArray().GetAt(_Idx);
		}
	}

	FbxVector4 Data = Uv->GetDirectArray().GetAt(Idx);

	_MeshData->UvVec[_Idx].x = (float)Data.mData[0];
	_MeshData->UvVec[_Idx].y = (float)(1.0f - Data.mData[1]);
}

void NTFBXLoader::FbxAniData(FbxMesh * _Mesh, NTFbxMeshData * _MeshData)
{
	int Count = _Mesh->GetDeformerCount(FbxDeformer::eSkin);

	if (0 >= Count)
	{
		_MeshData->bAnimated = false;
		return;
	}

	_MeshData->bAnimated = true;

	for (int i = 0; i < Count; i++)
	{
		FbxSkin* Skin = (FbxSkin*)_Mesh->GetDeformer(i, FbxDeformer::eSkin);

		if (nullptr == Skin)
		{
			continue;
		}

		FbxSkin::EType DType = Skin->GetSkinningType();

		if (FbxSkin::EType::eRigid == DType || DType == FbxSkin::EType::eLinear)
		{
			int ClusterCount = Skin->GetClusterCount();

			for (int j = 0; j < ClusterCount; j++)
			{
				FbxCluster* Cluster = Skin->GetCluster(j);

				if (nullptr == Cluster || nullptr == Cluster->GetLink())
				{
					continue;
				}

				NTBone* Bone = NewFbx->FindBone(CA2W(Cluster->GetLink()->GetName()).m_psz);

				if (nullptr == Bone)
				{
					tassert(true);
					return;
				}

				FbxAMatrix NodeTransform = GetFbxTransform(_Mesh->GetNode());
			}
		}
	}
}

void NTFBXLoader::FbxWeightsAndIndices()
{
}

void NTFBXLoader::FbxOffset()
{
}

void NTFBXLoader::FbxFrameMat()
{
}

FbxAMatrix NTFBXLoader::GetFbxTransform(FbxNode * _Node)
{
	const FbxVector4 T = _Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = _Node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = _Node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(T, R, S);
}

NTBone * NTFBX::FindBone(const wchar_t * _Name)
{
	std::multimap<std::wstring, NTBone*>::iterator FindIter = BoneMap.find(_Name);

	if (FindIter == BoneMap.end())
	{
		tassert(true);
		return nullptr;
	}

	return FindIter->second;
}
