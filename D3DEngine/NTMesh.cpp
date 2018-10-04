#include "PreCom.h"
#include "NTMesh.h"
#include "NTWinShortCut.h"


NTMesh::NTMesh() : VertexBuffer(nullptr), IndexBuffer(nullptr)
{
}


NTMesh::~NTMesh()
{
	if (nullptr != VertexBuffer)
	{
		VertexBuffer->Release();
	}

	if (nullptr != IndexBuffer)
	{
		IndexBuffer->Release();
	}
}

bool NTMesh::Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize,
	D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode)
{
	if (false == CreateVertex(_VertexCount, _VertexSize, _VertexUsage, _VertexMem))
	{
		return false;
	}

	if (false == CreateIndex(_IndexCount, _IndexSize, _IndexUsage, _IndexMem))
	{
		return false;
	}

	IndexFormat = _IndexFormat;
	DrawMode = _DrawMode;
	VertexSize = _VertexSize;
	IndexCount = _IndexCount;

	return true;
}

bool NTMesh::CreateVertex(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem)
{
	D3D11_BUFFER_DESC tBD = D3D11_BUFFER_DESC();

	tBD.ByteWidth = _VertexCount * _VertexSize;
	tBD.Usage = _VertexUsage;

	if (D3D11_USAGE_DYNAMIC == tBD.Usage)
	{
		tBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _VertexMem;

	if (S_OK != NTWinShortCut::GetDevice()->CreateBuffer(&tBD, &tSub, &VertexBuffer))
	{
		return false;
	}

	return true;
}

bool NTMesh::CreateIndex(UINT _TriangleCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem)
{
	D3D11_BUFFER_DESC tBD = D3D11_BUFFER_DESC();

	tBD.ByteWidth = _TriangleCount * _IndexSize;
	tBD.Usage = _IndexUsage;

	if (D3D11_USAGE_DYNAMIC == tBD.Usage)
	{
		tBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _IndexMem;

	if (S_OK != NTWinShortCut::GetDevice()->CreateBuffer(&tBD, &tSub, &IndexBuffer))
	{
		return false;
	}

	return true;
}

void NTMesh::Update()
{
	UINT Off = 0;
	UINT Size = VertexSize;

	NTWinShortCut::GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &Size, &Off);
	NTWinShortCut::GetContext()->IASetPrimitiveTopology(DrawMode);
	NTWinShortCut::GetContext()->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);
}

void NTMesh::Render()
{
	NTWinShortCut::GetContext()->DrawIndexed(IndexCount, 0, 0);
}