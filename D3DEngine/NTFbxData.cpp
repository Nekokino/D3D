#include "PreCom.h"
#include "NTFbxData.h"
#include "DirectValueHeader.h"
#include "NTReadStream.h"
#include "NTWriteStream.h"


NTFbxData::NTFbxData()
{
}


NTFbxData::~NTFbxData()
{
}

void NTFbxBinary::NTFbxConvert(NTFBX * _Data)
{
	for (size_t MeshIdx = 0; MeshIdx < _Data->MeshDataVec.size(); MeshIdx++)
	{
		NTFbxBiMeshData* BiMeshData = new NTFbxBiMeshData();
		NTFbxMeshData* MeshData = _Data->MeshDataVec[MeshIdx];

		BiMeshData->VtxCount = (UINT)MeshData->VtxVec.size();
		BiMeshData->VtxSize = sizeof(NTFbxVtxData);
		BiMeshData->Usage = D3D11_USAGE_DYNAMIC;
		BiMeshData->BDVtx = new char[BiMeshData->VtxCount * BiMeshData->VtxSize];
		memcpy_s(BiMeshData->BDVtx, BiMeshData->VtxCount * BiMeshData->VtxSize, &MeshData->VtxVec[0], BiMeshData->VtxCount * BiMeshData->VtxSize);

		BiMeshData->IdxVec.resize(MeshData->IdxVec.size());

		for (size_t SubIdx = 0; SubIdx < MeshData->IdxVec.size(); SubIdx++)
		{
			BiMeshData->IdxVec[SubIdx].IdxCount = (UINT)MeshData->IdxVec[SubIdx].size();
			BiMeshData->IdxVec[SubIdx].IdxSize = sizeof(DWORD);
			BiMeshData->IdxVec[SubIdx].Format = IDX32::GetFormat();
			BiMeshData->IdxVec[SubIdx].Usage = D3D11_USAGE_DEFAULT;
			BiMeshData->IdxVec[SubIdx].BDIdx = new char[BiMeshData->IdxVec[SubIdx].IdxCount * BiMeshData->IdxVec[SubIdx].IdxSize];

			memcpy_s(BiMeshData->IdxVec[SubIdx].BDIdx, BiMeshData->IdxVec[SubIdx].IdxCount * BiMeshData->IdxVec[SubIdx].IdxSize, &MeshData->IdxVec[SubIdx][0], BiMeshData->IdxVec[SubIdx].IdxCount * BiMeshData->IdxVec[SubIdx].IdxSize);
		}

		BiMeshData->MatVec.resize(MeshData->MatDataVec.size());

		memcpy_s(&BiMeshData->MatVec[0], sizeof(NTFbxMatData) * (UINT)MeshData->MatDataVec.size(), &MeshData->MatDataVec[0], sizeof(NTFbxMatData) * (UINT)MeshData->MatDataVec.size());

		MeshVec.push_back(BiMeshData);
	}

	AniVec.resize(_Data->AniVec.size());

	memcpy_s(&AniVec[0], sizeof(NTFbxAniInfo) * (UINT)_Data->AniVec.size(), &_Data->AniVec[0], sizeof(NTFbxAniInfo) * (UINT)_Data->AniVec.size());

	BoneVec.resize(_Data->BoneVec.size());

	for (size_t i = 0; i < _Data->BoneVec.size(); i++)
	{
		BoneVec[i] = new NTFbxBiBoneData();

		BoneVec[i]->BoneMat = _Data->BoneVec[i]->BoneMat;
		BoneVec[i]->OffsetMat = _Data->BoneVec[i]->OffsetMat;
		BoneVec[i]->Depth = _Data->BoneVec[i]->Depth;
		BoneVec[i]->Index = _Data->BoneVec[i]->Index;

		memcpy_s(BoneVec[i]->Name, sizeof(wchar_t) * 512, _Data->BoneVec[i]->Name, sizeof(wchar_t) * 512);

		BoneVec[i]->KeyFrameVec.resize(_Data->BoneVec[i]->KeyFrameVec.size());

		if (_Data->BoneVec[i]->KeyFrameVec.size() == 0)
		{
			continue;
		}

		memcpy_s(&BoneVec[i]->KeyFrameVec[0], sizeof(NTKeyFrame) * (UINT)BoneVec[i]->KeyFrameVec.size(), &_Data->BoneVec[i]->KeyFrameVec[0], sizeof(NTKeyFrame) * (UINT)BoneVec[i]->KeyFrameVec.size());

	}
}

void NTFbxBinary::SaveNTFbx(const wchar_t * _Path)
{
	NTWriteStream Stream = NTWriteStream(_Path);

	int MeshSize = (int)MeshVec.size();

	Stream.Write(MeshSize);

	for (size_t MeshIdx = 0; MeshIdx < MeshVec.size(); MeshIdx++)
	{
		NTFbxBiMeshData* BiMeshData = MeshVec[MeshIdx];

		Stream.Write(BiMeshData->VtxCount);
		Stream.Write(BiMeshData->VtxSize);
		Stream.Write(BiMeshData->Usage);
		Stream.Write(BiMeshData->BDVtx, BiMeshData->VtxCount * BiMeshData->VtxSize);

		int IdxSize = (int)MeshVec[MeshIdx]->IdxVec.size();

		Stream.Write(IdxSize);

		for (size_t SubIdx = 0; SubIdx < MeshVec[MeshIdx]->IdxVec.size(); SubIdx++)
		{
			NTFbxBiIndexData* BiIdxData = &(MeshVec[MeshIdx]->IdxVec[SubIdx]);

			Stream.Write(BiIdxData->IdxSize);
			Stream.Write(BiIdxData->IdxCount);
			Stream.Write(BiIdxData->Usage);
			Stream.Write(BiIdxData->Format);
			Stream.Write(BiIdxData->BDIdx, BiIdxData->IdxCount * BiIdxData->IdxSize);
		}

		int MatSize = (int)MeshVec[MeshIdx]->MatVec.size();

		Stream.Write(MatSize);

		Stream.Write(&MeshVec[MeshIdx]->MatVec[0], sizeof(NTFbxMatData) * (UINT)MeshVec[MeshIdx]->MatVec.size());
	}

	int AniSize = (int)AniVec.size();
	
	Stream.Write(AniSize);

	Stream.Write(&AniVec[0], sizeof(NTFbxAniInfo) * (UINT)AniSize);

	int BoneSize = (int)BoneVec.size();

	Stream.Write(BoneSize);

	for (int i = 0; i < BoneSize; i++)
	{
		Stream.Write(BoneVec[i]->Name, sizeof(wchar_t) * 512);
		Stream.Write(BoneVec[i]->BoneMat);
		Stream.Write(BoneVec[i]->OffsetMat);
		Stream.Write(BoneVec[i]->Depth);
		Stream.Write(BoneVec[i]->Index);

		int FrameSize = (int)BoneVec[i]->KeyFrameVec.size();
		Stream.Write(FrameSize);

		if (0 < FrameSize)
		{
			Stream.Write(&(BoneVec[i]->KeyFrameVec[0]), sizeof(NTKeyFrame) * FrameSize);
		}
	}
}

void NTFbxBinary::LoadNTFbx(const wchar_t * _Path)
{
	NTReadStream Stream = NTReadStream(_Path);

	int MeshSize;
	Stream.Read(MeshSize);
	MeshVec.resize(MeshSize);

	for (size_t MeshIdx = 0; MeshIdx < MeshVec.size(); MeshIdx++)
	{
		MeshVec[MeshIdx] = new NTFbxBiMeshData();

		Stream.Read(MeshVec[MeshIdx]->VtxCount);
		Stream.Read(MeshVec[MeshIdx]->VtxSize);
		Stream.Read(MeshVec[MeshIdx]->Usage);

		MeshVec[MeshIdx]->BDVtx = new char[MeshVec[MeshIdx]->VtxCount * MeshVec[MeshIdx]->VtxSize];
		Stream.Read(MeshVec[MeshIdx]->BDVtx, MeshVec[MeshIdx]->VtxCount * MeshVec[MeshIdx]->VtxSize);

		int IdxSize;
		Stream.Read(IdxSize);
		MeshVec[MeshIdx]->IdxVec.resize(IdxSize);

		for (size_t SubIdx = 0; SubIdx < MeshVec[MeshIdx]->IdxVec.size(); SubIdx++)
		{
			NTFbxBiIndexData* NewIdxData = &(MeshVec[MeshIdx]->IdxVec[SubIdx]);

			Stream.Read(NewIdxData->IdxSize);
			Stream.Read(NewIdxData->IdxCount);
			Stream.Read(NewIdxData->Usage);
			Stream.Read(NewIdxData->Format);

			NewIdxData->BDIdx = new char[NewIdxData->IdxSize * NewIdxData->IdxCount];
			Stream.Read(NewIdxData->BDIdx, NewIdxData->IdxSize * NewIdxData->IdxCount);
		}

		int MatSize;
		Stream.Read(MatSize);
		MeshVec[MeshIdx]->MatVec.resize(MatSize);
		Stream.Read(&MeshVec[MeshIdx]->MatVec[0], sizeof(NTFbxMatData) * (UINT)MatSize);
	}

	int AniSize;
	Stream.Read(AniSize);
	AniVec.resize(AniSize);
	Stream.Read(&AniVec[0], sizeof(NTFbxAniInfo) * (UINT)AniSize);

	int BoneSize;
	Stream.Read(BoneSize);
	BoneVec.resize(BoneSize);

	for (int i = 0; i < BoneSize; i++)
	{
		BoneVec[i] = new NTFbxBiBoneData();

		Stream.Read(BoneVec[i]->Name, sizeof(wchar_t) * 512);
		Stream.Read(BoneVec[i]->BoneMat);
		Stream.Read(BoneVec[i]->OffsetMat);
		Stream.Read(BoneVec[i]->Depth);
		Stream.Read(BoneVec[i]->Index);

		int FrameSize;
		Stream.Read(FrameSize);

		if (0 < FrameSize)
		{
			BoneVec[i]->KeyFrameVec.resize(FrameSize);
			Stream.Read(&(BoneVec[i]->KeyFrameVec[0]), sizeof(NTKeyFrame) * FrameSize);
		}
	}
}

bool NTFbxData::Load(LOADMODE _Mode)
{
	if (LOADMODE::FBXMODE == _Mode)
	{
		NTFbxLoader Loader;
		Loader.LoadFbx(GetPath());
		Data.NTFbxConvert(Loader.NewFbx);
		return true;
	}
	else
	{
		Data.LoadNTFbx(GetPath());
	}

	return true;
}

void NTFbxData::SaveBinaryData(const wchar_t* _Path)
{
	Data.SaveNTFbx(_Path);
}