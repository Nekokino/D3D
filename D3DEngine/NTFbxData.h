#pragma once
#include "NTFBX.h"
#include "NTResource.h"

enum LOADMODE
{
	FBXMODE,
	BIMODE,
};

class NTFbxBiBoneData
{
public:
	wchar_t Name[512] = {0,};
	unsigned int Depth;
	unsigned int Index;
	NTMAT OffsetMat;
	NTMAT BoneMat;

	std::vector<NTKeyFrame> KeyFrameVec;
};

class NTFbxBiIndexData
{
public:
	unsigned int IdxSize;
	unsigned int IdxCount;
	D3D11_USAGE Usage;
	DXGI_FORMAT Format;
	char* BDIdx;

public:
	~NTFbxBiIndexData() { delete BDIdx; }
};

class NTFbxBiMeshData
{
public:
	unsigned int VtxSize;
	unsigned int VtxCount;
	D3D11_USAGE Usage;
	char* BDVtx;

	std::vector<NTFbxBiIndexData> IdxVec;
	std::vector<NTFbxMatData> MatVec;

public:
	~NTFbxBiMeshData() { delete BDVtx; }
};

class NTFbxBinary
{
public:
	std::vector<NTFbxBiMeshData*> MeshVec;
	std::vector<NTFbxAniInfo> AniVec;
	std::vector<NTFbxBiBoneData*> BoneVec;

	std::map<std::wstring, NTFbxBiBoneData*> BoneMap;

public:
	void NTFbxConvert(NTFBX* _Data);
	void SaveNTFbx(const wchar_t* _Path);
	void LoadNTFbx(const wchar_t* _Path);

public:
	~NTFbxBinary()
	{
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			delete BoneVec[i];
		}

		for (size_t i = 0; i < MeshVec.size(); i++)
		{
			delete MeshVec[i];
		}
	}
};

class NTFbxData : public NTResource
{
public:
	NTFbxBinary Data;

public:
	bool Load(LOADMODE _Mode = LOADMODE::BIMODE);
	void SaveBinaryData(const wchar_t* _Path);
	NTFbxBiBoneData* FindBone(const wchar_t* _Name);
public:
	NTFbxData();
	~NTFbxData();
};

