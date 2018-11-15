#pragma once

#include <fbxsdk.h>
#include <vector>
#include <string>
#include <map>

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_D32")
	#else
		#pragma comment(lib, "libfbxsdk-md_D64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "libfbxsdk-md_R32")
	#else
		#pragma comment(lib, "libfbxsdk-md_R64")
	#endif
#endif

class NTKeyFrame
{
public:
	FbxMatrix BoneMat;
	double Time;
};

class NTBone
{
public:
	std::wstring Name;
	unsigned int Depth;
	unsigned int Index;
	NTBone* Parent;
	FbxMatrix OffsetMat;
	FbxMatrix BoneMat;
	std::vector<NTKeyFrame> KeyFrameVec;
};

class NTFbxMatData
{

};

class NTFbxMeshData
{
public:
	std::wstring Name;

	std::vector<NTVEC> PosVec;
	std::vector<NTVEC> NormalVec;
	std::vector<NTVEC> TangentVec;
	std::vector<NTVEC> BiNormalVec;
	std::vector<NTVEC2> UvVec;

	std::vector<std::vector<UINT>> IdxVec;

	NTFbxMatData MatData;

	bool bAnimated;

	std::vector<NTVEC> IndicesVec;
	std::vector<NTVEC> WeightsVec;

	void SetVtxCount(UINT _Count)
	{
		PosVec.resize(_Count);
		NormalVec.resize(_Count);
		TangentVec.resize(_Count);
		BiNormalVec.resize(_Count);
		UvVec.resize(_Count);
		IndicesVec.resize(_Count);
		WeightsVec.resize(_Count);
	}
};

class NTFbxAniInfo
{
public:
	std::wstring AniName;
	FbxTime StartTime;
	FbxTime EndTime;
	FbxLongLong TimeLength;
	FbxTime::EMode Mode;
};

class NTFBX
{
public:
	std::vector<NTBone*> BoneVec;
	std::multimap<std::wstring, NTBone*> BoneMap;

	FbxArray<FbxString*> AniNameArray;
	std::map<std::wstring, NTFbxAniInfo*> AniMap;

	std::vector<NTFbxMeshData*> MeshDataVec;

public:
	NTBone* FindBone(const wchar_t* _Name);

public:
	~NTFBX()
	{
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			delete BoneVec[i];
		}

		for (size_t i = 0; i < MeshDataVec.size(); i++)
		{
			delete MeshDataVec[i];
		}
	}
};

class NTFBXLoader
{
public:
	FbxManager* Manager;
	FbxScene* Scene;
	NTFBX* NewFbx;
	unsigned int BoneCount;
public:
	// 기본 데이터
	void Load(const wchar_t* _Path);
	void CalBoneCount(FbxNode* _Node);

	// 본 정보
	void LoadBone(FbxNode* _Node, unsigned int _Depth, NTBone* _Parent);

	// 애니메이션 정보
	void AniCheck();
	void Triangulate(FbxNode* _Node);

	// 메쉬(& 정점)
	void FbxMeshData(FbxNode* _Node);
	void FbxNormal(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxTangent(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxBiNormal(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxUv(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);

	// 정점에 애니메이션 정보 넣기
	void FbxAniData(FbxMesh* _Mesh, NTFbxMeshData* _MeshData);
	void FbxWeightsAndIndices();
	void FbxOffset();
	void FbxFrameMat();

	FbxAMatrix GetFbxTransform(FbxNode* _Node);
public:
	NTFBXLoader();
	~NTFBXLoader();
};