#include "PreCom.h"
#include "NTMesh.h"
#include "NTWinShortCut.h"


NTMesh::NTMesh() : DrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}


NTMesh::~NTMesh()
{
}

bool NTMesh::Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize,
	D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode)
{
	if (false == CreateVertex(_VertexCount, _VertexSize, _VertexUsage, _VertexMem))
	{
		return false;
	}

	if (false == CreateIndex(_IndexCount, _IndexSize, _IndexUsage, _IndexFormat, _IndexMem))
	{
		return false;
	}

	DrawMode = _DrawMode;

	return true;
}

bool NTMesh::CreateVertex(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem)
{
	NTVertexBuffer* Info = new NTVertexBuffer();

	Info->VtxCount = _VertexCount;
	Info->VtxSize = _VertexSize;
	Info->BufDesc.ByteWidth = _VertexCount * _VertexSize;
	Info->BufDesc.Usage = _VertexUsage;

	if (D3D11_USAGE_DYNAMIC == Info->BufDesc.Usage)
	{
		Info->BufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	Info->BufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _VertexMem;

	if (S_OK != NTWinShortCut::GetDevice()->CreateBuffer(&Info->BufDesc, &tSub, &Info->VtxBuf))
	{
		tassert(true);
		return false;
	}

	BufSizeVec.push_back(Info->VtxSize);
	VtxBufVec.push_back(Info->VtxBuf);
	VtxBufInfoVec.push_back(Info);

	return true;
}

bool NTMesh::CreateIndex(UINT _IndexCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, DXGI_FORMAT _Format, void* _IndexMem)
{
	NTIndexBuffer* Info = new NTIndexBuffer();

	Info->IdxCount = _IndexCount;
	Info->IdxSize = _IndexSize;
	Info->IdxFormat = _Format;
	Info->BufDesc.ByteWidth = _IndexCount * _IndexSize;
	Info->BufDesc.Usage = _IndexUsage;

	if (D3D11_USAGE_DYNAMIC == Info->BufDesc.Usage)
	{
		Info->BufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	Info->BufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _IndexMem;

	if (S_OK != NTWinShortCut::GetDevice()->CreateBuffer(&Info->BufDesc, &tSub, &Info->IdxBuf))
	{
		tassert(true);
		return false;
	}

	IdxBufVec.push_back(Info);

	return true;
}

void NTMesh::Update(UINT _StartVtx, UINT _VtxCount, UINT* _Off)
{
	UINT Idx = 0;
	UINT* Off = _Off;

	if (nullptr == _Off)
	{
		Off = &Idx;
	}

	NTWinShortCut::GetContext()->IASetVertexBuffers(_StartVtx, _VtxCount, &VtxBufVec[0], &BufSizeVec[0], Off);
	NTWinShortCut::GetContext()->IASetPrimitiveTopology(DrawMode);

}

void NTMesh::Render(UINT _StartIdx, UINT _EndIndex, UINT* _Off)
{
	UINT Idx = 0;
	UINT* Off = _Off;

	if (nullptr == _Off)
	{
		Off = &Idx;
	}

	for (size_t i = _StartIdx; i < _EndIndex; i++)
	{
		NTWinShortCut::GetContext()->IASetIndexBuffer(IdxBufVec[i]->IdxBuf, IdxBufVec[i]->IdxFormat, Off[i]);
		NTWinShortCut::GetContext()->DrawIndexed(IdxBufVec[i]->IdxCount, 0, 0);
	}
}

void NTMesh::UpdateAndRender(UINT _Vtx, UINT _Sub)
{
	UINT Idx = 0;
	NTWinShortCut::GetContext()->IASetVertexBuffers(_Vtx, 1, &VtxBufVec[0], &BufSizeVec[0], &Idx);
	NTWinShortCut::GetContext()->IASetPrimitiveTopology(DrawMode);
	NTWinShortCut::GetContext()->IASetIndexBuffer(IdxBufVec[_Sub]->IdxBuf, IdxBufVec[_Sub]->IdxFormat, 0);
	NTWinShortCut::GetContext()->DrawIndexed(IdxBufVec[_Sub]->IdxCount, 0, 0);
}

void NTMesh::SetVtxData(UINT _BufIdx, void* _VtxMem, UINT _VtxSize)
{
	if (VtxBufInfoVec.size() <= _BufIdx)
	{
		tassert(true);

		return;
	}

	VtxBufInfoVec[_BufIdx]->SetVtxData(_VtxMem, _VtxSize);
}

void NTVertexBuffer::SetVtxData(void * _VtxMem, UINT _VtxSize)
{
	D3D11_MAPPED_SUBRESOURCE Sub = {};

	NTWinShortCut::GetContext()->Map(VtxBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &Sub);

	memcpy(Sub.pData, &_VtxMem, _VtxSize);

	NTWinShortCut::GetContext()->Unmap(VtxBuf, 0);
}
