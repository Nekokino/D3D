#include "PreCom.h"
#include "NTFbxData.h"
#include "DirectValueHeader.h"


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
	}
}

bool NTFbxData::Load(bool _bLoad)
{
	return false;
}

void NTFbxData::SaveBinaryData()
{
}