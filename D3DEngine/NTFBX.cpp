#include "PreCom.h"
#include "DirectValueHeader.h"
#include <atlstr.h>
#include <algorithm>
#include "NTFBX.h"

FbxAMatrix NTFbxLoader::MatReflect;

NTFbxLoader::NTFbxLoader() : Manager(nullptr), Scene(nullptr), NewFbx(nullptr), BoneCount(0)
{
}

NTFbxLoader::~NTFbxLoader()
{
	if (nullptr != NewFbx)
	{
		delete NewFbx;
	}
}

void NTFbxLoader::FbxInit()
{
	MatReflect[0] = FbxVector4{ 1, 0, 0, 0 };
	MatReflect[1] = FbxVector4{ 0, 0, 1, 0 };
	MatReflect[2] = FbxVector4{ 0, 1, 0, 0 };
	MatReflect[3] = FbxVector4{ 0, 0, 0, 1 };
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


NTMAT NTFbxLoader::FbxMatConvert(const FbxMatrix & _Mat)
{
	NTMAT Return;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			Return.m[y][x] = (float)_Mat.Get(y, x);
		}
	}

	return Return;
}

FbxMatrix NTFbxLoader::NTMATConvert(const NTMAT & _Mat)
{
	FbxMatrix Return;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			Return.Set(y, x, _Mat.m[y][x]);
		}
	}

	return Return;
}

NTVEC NTFbxLoader::FbxVec4Convert(const FbxVector4 & _Vec)
{
	NTVEC Return;

	for (int i = 0; i < 4; ++i)
	{
		Return.s[i] = (float)_Vec.mData[i];
	}

	return Return;
}

NTVEC NTFbxLoader::FbxQuaternionConvert(const FbxQuaternion & _Q)
{
	NTVEC Return;

	for (int i = 0; i < 4; ++i)
	{
		Return.s[i] = (float)_Q.mData[i];
	}

	return Return;
}

NTVEC NTFbxLoader::GetMaterialColor(FbxSurfaceMaterial * _FbxMatData, const char * _MaterialColorName, const char * _MaterialFactorName)
{
	FbxDouble3 FbxColor;
	FbxDouble Factor = 0;

	FbxProperty MaterialColorPro = _FbxMatData->FindProperty(_MaterialColorName);
	FbxProperty MaterialFactorPro = _FbxMatData->FindProperty(_MaterialFactorName);

	if (MaterialColorPro.IsValid())
	{
		FbxColor = MaterialColorPro.Get<FbxDouble3>();
	}

	if (MaterialFactorPro.IsValid())
	{
		Factor = MaterialFactorPro.Get<FbxDouble>();
	}

	return NTVEC((float)(FbxColor.mData[0] * Factor), (float)(FbxColor.mData[1] * Factor), (float)(FbxColor.mData[2] * Factor), (float)Factor);
}

std::wstring NTFbxLoader::GetMaterialTextureName(FbxSurfaceMaterial * _FbxMaterialData, const char * _MaterialTextureName)
{
	std::string Name = "";

	FbxProperty MaterialTextureNamePro = _FbxMaterialData->FindProperty(_MaterialTextureName);

	if (MaterialTextureNamePro.IsValid())
	{
		UINT Count = MaterialTextureNamePro.GetSrcObjectCount();

		if (0 != Count)
		{
			FbxFileTexture* FbxTexture = MaterialTextureNamePro.GetSrcObject<FbxFileTexture>(0);
			if (nullptr != FbxTexture)
			{
				Name = FbxTexture->GetFileName();
			}
		}
	}

	return CA2W(Name.c_str(), CP_UTF8).m_psz;
}


void NTFbxLoader::LoadFbx(const wchar_t * _Path)
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
}

void NTFbxLoader::LoadFbxBone(const wchar_t * _Path)
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

	Importer->Destroy();
	Scene->Destroy();
	SetP->Destroy();
	Manager->Destroy();
}

void NTFbxLoader::CalBoneCount(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();
	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		++BoneCount;
	}
	int Count = _Node->GetChildCount();
	for (int i = 0; i < Count; i++)
	{
		CalBoneCount(_Node->GetChild(i));
	}
}

void NTFbxLoader::LoadBone(FbxNode* _Node, unsigned int _Depth, NTBone * _Parent)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	NTBone* NewBone = nullptr;

	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		NewBone = new NTBone();
		std::wstring Name = CA2W(_Node->GetName(), CP_UTF8).m_psz;
		memcpy_s(NewBone->Name, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);
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

void NTFbxLoader::AniCheck()
{
	FbxArray<FbxString*> AniNameArray;

	Scene->FillAnimStackNameArray(AniNameArray);

	NewFbx->AniVec.resize(AniNameArray.GetCount());

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		FbxAnimStack* AniStack = Scene->FindMember<FbxAnimStack>(AniNameArray[i]->Buffer());

		if (nullptr == AniStack)
		{
			continue;
		}

		NTFbxAniInfo NewInfo;

		std::wstring Name = CA2W(AniStack->GetName(), CP_UTF8).m_psz;
		memcpy_s(NewInfo.AniName, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AniStack->GetName());
		NewInfo.StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		NewInfo.EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		NewInfo.Mode = Scene->GetGlobalSettings().GetTimeMode();
		NewInfo.TimeLength = NewInfo.EndTime.GetFrameCount(NewInfo.Mode) - NewInfo.StartTime.GetFrameCount(NewInfo.Mode);

		NewInfo.Index = i;
		NewFbx->AniVec[i] = NewInfo;
		NewFbx->AniMap.insert(std::map<std::wstring, NTFbxAniInfo*>::value_type(NewInfo.AniName, &NewFbx->AniVec[i]));
	}

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		delete AniNameArray[i];
	}
}

void NTFbxLoader::Triangulate(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
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

void NTFbxLoader::FbxMeshCon(NTFbxMeshData * _MeshData, FbxMesh * _Mesh)
{
	int VtxCount = _Mesh->GetControlPointsCount();
	_MeshData->SetVtxCount(VtxCount);

	FbxVector4* FbxPos = _Mesh->GetControlPoints();

	for (int i = 0; i < VtxCount; i++)
	{
		_MeshData->VtxVec[i].Pos.x = (float)FbxPos[i].mData[0];
		_MeshData->VtxVec[i].Pos.y = (float)FbxPos[i].mData[2];
		_MeshData->VtxVec[i].Pos.z = (float)FbxPos[i].mData[1];
		_MeshData->VtxVec[i].Pos.w = 1.0f;
	}

	int TriCount = _Mesh->GetPolygonCount();

	int IdxSize = _Mesh->GetPolygonSize(0);

	if (3 != IdxSize)
	{
		tassert(true);
		return;
	}

	IDX32 TmpIDX = {};
	UINT CurVtx = 0;

	FbxGeometryElementMaterial* Mtrl = _Mesh->GetElementMaterial();

	for (int i = 0; i < TriCount; i++)
	{
		for (int j = 0; j < IdxSize; j++)
		{
			TmpIDX.p[j] = (DWORD)_Mesh->GetPolygonVertex(i, j);
			FbxNormal(_Mesh, _MeshData, TmpIDX.p[j], CurVtx);
			FbxTangent(_Mesh, _MeshData, TmpIDX.p[j], CurVtx);
			FbxUv(_Mesh, _MeshData, TmpIDX.p[j], CurVtx);
			FbxBiNormal(_Mesh, _MeshData, TmpIDX.p[j], CurVtx);
			++CurVtx;
		}

		int SubIdx = Mtrl->GetIndexArray().GetAt(i);
		_MeshData->IdxVec[SubIdx].push_back(TmpIDX.p[0]);
		_MeshData->IdxVec[SubIdx].push_back(TmpIDX.p[2]);
		_MeshData->IdxVec[SubIdx].push_back(TmpIDX.p[1]);
	}
}

void NTFbxLoader::FbxMaterial(NTFbxMeshData* _MeshData, FbxSurfaceMaterial* _SurMat)
{
	NTFbxMatData NewMat;

	NewMat.Info.Diffuse = GetMaterialColor(_SurMat, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	NewMat.Info.Ambient = GetMaterialColor(_SurMat, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	NewMat.Info.Emissive = GetMaterialColor(_SurMat, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	NewMat.Info.Specular = GetMaterialColor(_SurMat, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	std::wstring Name = GetMaterialTextureName(_SurMat, FbxSurfaceMaterial::sDiffuse);
	memcpy_s(NewMat.Diffuse, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);
	Name = GetMaterialTextureName(_SurMat, FbxSurfaceMaterial::sEmissive);
	memcpy_s(NewMat.Emissive, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);
	Name = GetMaterialTextureName(_SurMat, FbxSurfaceMaterial::sNormalMap);
	memcpy_s(NewMat.Bump, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);
	Name = GetMaterialTextureName(_SurMat, FbxSurfaceMaterial::sSpecular);
	memcpy_s(NewMat.Specular, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);

	_MeshData->MatDataVec.push_back(NewMat);
}

void NTFbxLoader::FbxMeshData(FbxNode * _Node)
{
	FbxNodeAttribute* Attr = _Node->GetNodeAttribute();

	if (nullptr != Attr && Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* NewMesh = _Node->GetMesh();

		if (nullptr != NewMesh)
		{
			NTFbxMeshData* NewMeshData = new NTFbxMeshData();
			NewFbx->MeshDataVec.push_back(NewMeshData);

			std::wstring Name = CA2W(NewMesh->GetName(), CP_UTF8).m_psz;
			memcpy_s(NewMeshData->Name, sizeof(wchar_t) * 512, Name.c_str(), Name.size() * 2);

			int MtrCount = _Node->GetMaterialCount();

			NewMeshData->IdxVec.resize(MtrCount);

			NewMeshData->MatDataVec.reserve(MtrCount);

			// 메시 데이터 얻으옴
			FbxMeshCon(NewMeshData, NewMesh);

			// 애니메이션 정보 얻으옴
			if (false == NewFbx->AniMap.empty())
			{
				FbxAniData(NewMesh, NewMeshData);
			}
			else
			{
				NewMeshData->bAnimated = false;
			}

			for (int i = 0; i < MtrCount; i++)
			{
				FbxMaterial(NewMeshData, _Node->GetMaterial(i));
			}
		}
	}

	int ChildCount = _Node->GetChildCount();
	for (int i = 0; i < ChildCount; i++)
	{
		FbxMeshData(_Node->GetChild(i));
	}
}

void NTFbxLoader::FbxNormal(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
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

	_MeshData->VtxVec[_Idx].Normal.x = (float)Data.mData[0];
	_MeshData->VtxVec[_Idx].Normal.y = (float)Data.mData[2];
	_MeshData->VtxVec[_Idx].Normal.z = (float)Data.mData[1];
	_MeshData->VtxVec[_Idx].Normal.w = 0.0f;

}

void NTFbxLoader::FbxTangent(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementTangentCount();

	if (1 != Count)
	{
		//tassert(true);
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

	_MeshData->VtxVec[_Idx].Tangent.x = (float)Data.mData[0];
	_MeshData->VtxVec[_Idx].Tangent.y = (float)Data.mData[2];
	_MeshData->VtxVec[_Idx].Tangent.z = (float)Data.mData[1];
	_MeshData->VtxVec[_Idx].Tangent.w = 0.0f;
}

void NTFbxLoader::FbxBiNormal(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementBinormalCount();

	if (1 != Count)
	{
		//tassert(true);
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

	_MeshData->VtxVec[_Idx].BiNormal.x = (float)Data.mData[0];
	_MeshData->VtxVec[_Idx].BiNormal.y = (float)Data.mData[2];
	_MeshData->VtxVec[_Idx].BiNormal.z = (float)Data.mData[1];
	_MeshData->VtxVec[_Idx].BiNormal.w = 0.0f;
}

void NTFbxLoader::FbxUv(FbxMesh * _Mesh, NTFbxMeshData * _MeshData, DWORD _Idx, DWORD _Vtx)
{
	int Count = _Mesh->GetElementUVCount();

	if (1 != Count)
	{
		//tassert(true);
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

	_MeshData->VtxVec[_Idx].Uv.x = (float)Data.mData[0];
	_MeshData->VtxVec[_Idx].Uv.y = (float)(1.0f - Data.mData[1]);
}

void NTFbxLoader::FbxAniData(FbxMesh * _Mesh, NTFbxMeshData * _MeshData)
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
				// 결합부위 정점이 클러스터
				// 클러스터가 모여서 조인트가 된다.
				FbxCluster* Cluster = Skin->GetCluster(j);

				if (nullptr == Cluster || nullptr == Cluster->GetLink())
				{
					continue;
				}

				NTBone* Bone = NewFbx->FindBone(CA2W(Cluster->GetLink()->GetName(), CP_UTF8).m_psz);

				if (nullptr == Bone)
				{
					tassert(true);
					return;
				}

				Bone->BoneAMat = GetFbxTransform(_Mesh->GetNode());
				Bone->BoneMat = FbxMatConvert(Bone->BoneAMat);

				FbxWeightsAndIndices(Cluster, Bone, _MeshData);
				FbxOffset(Cluster, Bone, _MeshData);
				FbxFrameMat(_Mesh->GetNode(), Cluster, Bone, _MeshData);
			}
		}
	}

	WICheck(_Mesh, _MeshData);
}

void NTFbxLoader::FbxWeightsAndIndices(FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData)
{
	int IndexCount = _Cluster->GetControlPointIndicesCount();

	WI Tmp;

	for (int i = 0; i < IndexCount; i++)
	{
		Tmp.BoneIdx = _Bone->Index;
		Tmp.Weights = _Cluster->GetControlPointWeights()[i];
		Tmp.Indices = _Cluster->GetControlPointIndices()[i];
		_MeshData->WIVec[Tmp.Indices].push_back(Tmp);
	}
}

void NTFbxLoader::FbxOffset(FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData)
{
	FbxAMatrix ClusterTransform;
	FbxAMatrix ClusterLinkTransform;

	_Cluster->GetTransformMatrix(ClusterTransform);
	_Cluster->GetTransformLinkMatrix(ClusterLinkTransform);

	FbxAMatrix Offset;

	// 클러스터 단위의 오프셋 행렬을 구하는 과정
	// 노드와 링크된 다른 행렬의 역행렬 * 나의 행렬 * 본의 베이스 행렬

	Offset = ClusterLinkTransform.Inverse() * ClusterTransform * _Bone->BoneAMat;


	// 프로젝트의 축의 방향과 Max의 축의 방향이 다르므로 보정해준다.
	Offset = MatReflect * Offset * MatReflect;

	_Bone->OffsetAMat = Offset;
	_Bone->OffsetMat = FbxMatConvert(Offset);
}

void NTFbxLoader::FbxFrameMat(FbxNode* _Node, FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData)
{
	FbxTime::EMode TimeMode = Scene->GetGlobalSettings().GetTimeMode();

	for (size_t i = 0; i < NewFbx->AniVec.size(); i++)
	{
		FbxLongLong StartFrame = NewFbx->AniVec[i].StartTime.GetFrameCount(TimeMode);
		FbxLongLong EndFrame = NewFbx->AniVec[i].EndTime.GetFrameCount(TimeMode);

		for (FbxLongLong i = StartFrame; i < EndFrame; ++i)
		{
			NTKeyFrame Frame = {};
			FbxTime Time = 0;

			Time.SetFrame(i, TimeMode);

			FbxAMatrix TransformMat = _Node->EvaluateGlobalTransform(Time) * _Bone->BoneAMat;
			FbxAMatrix TransformCur = TransformMat.Inverse() * _Cluster->GetLink()->EvaluateGlobalTransform(Time);

			TransformCur = MatReflect * TransformCur * MatReflect;

			Frame.Time = Time.GetSecondDouble();
			//Frame.FrameMat = TransformCur;

			Frame.S = NTFbxLoader::FbxVec4Convert(TransformCur.GetS());
			Frame.T = NTFbxLoader::FbxVec4Convert(TransformCur.GetT());
			Frame.Q = NTFbxLoader::FbxQuaternionConvert(TransformCur.GetQ());

			NewFbx->BoneVec[_Bone->Index]->KeyFrameVec.push_back(Frame);
		}
	}
}

void NTFbxLoader::WICheck(FbxMesh * _Mesh, NTFbxMeshData * _MeshData)
{
	std::vector<std::vector<WI>>::iterator Iter = _MeshData->WIVec.begin();

	int VtxIndex = 0;

	for (VtxIndex = 0; Iter != _MeshData->WIVec.end(); ++Iter, ++VtxIndex)
	{
		if ((*Iter).size() > 1)
		{
			std::sort((*Iter).begin(), (*Iter).end(), [](const WI& _Left, const WI& _Right)
			{
				return _Left.Weights > _Right.Weights;
			}
			);

			double Weight = 0.0f;
			for (int i = 0; i < (int)(*Iter).size(); i++)
			{
				Weight += (*Iter)[i].Weights;
			}

			double Revision = 0.0f;
			if (Weight > 1.0f)
			{
				Revision = 1.0f - Weight;
				(*Iter)[0].Weights += Revision;
			}

			if ((*Iter).size() >= 4)
			{
				(*Iter).erase((*Iter).begin() + 4, (*Iter).end());
			}
		}

		for (size_t i = 0; i < (*Iter).size(); i++)
		{
			_MeshData->VtxVec[VtxIndex].Weights.s[i] = (float)(*Iter)[i].Weights;
			_MeshData->VtxVec[VtxIndex].Indices.s[i] = (float)(*Iter)[i].BoneIdx;
		}
	}
}

FbxAMatrix NTFbxLoader::GetFbxTransform(FbxNode * _Node)
{
	const FbxVector4 T = _Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = _Node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = _Node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(T, R, S);
}