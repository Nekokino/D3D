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
	NTVEC S;
	NTVEC T;
	NTVEC Q;
	double Time;
};

class NTBone
{
public:
	wchar_t Name[512] = {};
	unsigned int Depth;
	unsigned int Index;
	NTBone* Parent;
	NTMAT OffsetMat;
	NTMAT BoneMat;
	FbxAMatrix OffsetAMat;
	FbxAMatrix BoneAMat;
	std::vector<NTKeyFrame> KeyFrameVec;
};

class MaterialInfo
{
public:
	NTVEC Diffuse;
	NTVEC Specular;
	NTVEC Ambient;
	NTVEC Emissive;

public:
	MaterialInfo() : Diffuse(NTVEC::ONE), Specular(NTVEC::ONE), Ambient(NTVEC::ONE), Emissive(NTVEC::ONE) {}
};

class NTFbxMatData
{
public:
	MaterialInfo Info;
	wchar_t Name[512] = {};
	wchar_t Diffuse[512] = {};
	wchar_t Bump[512] = {};
	wchar_t Specular[512] = {};
	wchar_t Emissive[512] = {};
};

class WI // Weights And Indices
{
public:
	int BoneIdx;
	int Indices;
	double Weights;
};

class NTFbxVtxData
{
public:
	NTVEC Pos;
	NTVEC2 Uv;
	NTVEC Color;
	NTVEC Normal;
	NTVEC Tangent;
	NTVEC BiNormal;
	NTVEC Weights;
	NTVEC Indices;

	NTFbxVtxData() : Color(NTVEC::ONE)
	{

	}
};

class NTFbxMeshData
{
public:
	wchar_t Name[512] = {};

	std::vector<NTFbxVtxData> VtxVec;

	std::vector<std::vector<UINT>> IdxVec;

	std::vector<NTFbxMatData> MatDataVec;

	std::vector<std::vector<WI>> WIVec;

	bool bAnimated;

	void SetVtxCount(UINT _Count)
	{
		VtxVec.resize(_Count);
		WIVec.resize(_Count);
	}
};

class NTFbxAniInfo
{
public:
	int Index;
	wchar_t AniName[512] = {};
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

	std::vector<NTFbxAniInfo> AniVec;
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

class NTFbxLoader
{
public:
	static void FbxInit();
	static NTMAT FbxMatConvert(const FbxMatrix& _Mat);
	static FbxMatrix NTMATConvert(const NTMAT& _Mat);
	static NTVEC FbxVec4Convert(const FbxVector4& _Vec);
	static NTVEC FbxQuaternionConvert(const FbxQuaternion& _Q);

private:
	static FbxAMatrix MatReflect;
public:
	FbxManager* Manager;
	FbxScene* Scene;
	NTFBX* NewFbx;
	unsigned int BoneCount;
public:
	// 기본 데이터
	void LoadFbx(const wchar_t* _Path);

private:
	void CalBoneCount(FbxNode* _Node);

	// 본 정보
	void LoadBone(FbxNode* _Node, unsigned int _Depth, NTBone* _Parent);

	// 애니메이션 정보
	void AniCheck();
	void Triangulate(FbxNode* _Node);

	void FbxMaterial(NTFbxMeshData* _MeshData, FbxSurfaceMaterial* _SurMat);
	void FbxMeshCon(NTFbxMeshData* _MeshData, FbxMesh* _Mesh);

	// 메쉬(& 정점)
	void FbxMeshData(FbxNode* _Node);
	void FbxNormal(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxTangent(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxBiNormal(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);
	void FbxUv(FbxMesh* _Mesh, NTFbxMeshData* _MeshData, DWORD _Idx, DWORD _Vtx);

	// 정점에 애니메이션 정보 넣기
	void FbxAniData(FbxMesh* _Mesh, NTFbxMeshData* _MeshData);
	void FbxWeightsAndIndices(FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData);
	void FbxOffset(FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData);
	void FbxFrameMat(FbxNode* _Node, FbxCluster* _Cluster, NTBone* _Bone, NTFbxMeshData* _MeshData);
	void WICheck(FbxMesh* _Mesh, NTFbxMeshData* _MeshData);

	FbxAMatrix GetFbxTransform(FbxNode* _Node);

	NTVEC GetMaterialColor(FbxSurfaceMaterial* _FbxMatData, const char* _MaterialColorName, const char* _MaterialFactorName);
	std::wstring GetMaterialTextureName(FbxSurfaceMaterial* _FbxMaterialData, const char* _MaterialTextureName);
public:
	NTFbxLoader();
	~NTFbxLoader();
};