#pragma once
#include "NTResource.h"
#include <vector>

class NTVertexBuffer : public RefCounter
{
public:
	ID3D11Buffer* VtxBuf;
	UINT VtxSize;
	UINT VtxCount;
	D3D11_BUFFER_DESC BufDesc = D3D11_BUFFER_DESC();
	NTVertexBuffer() : VtxBuf(nullptr) {}
	~NTVertexBuffer()
	{
		if (nullptr != VtxBuf)
		{
			VtxBuf->Release();
		}
	}
};

class NTIndexBuffer : public RefCounter
{
public:
	ID3D11Buffer* IdxBuf;
	UINT IdxSize;
	UINT IdxCount;
	DXGI_FORMAT IdxFormat;
	D3D11_BUFFER_DESC BufDesc = D3D11_BUFFER_DESC();
	NTIndexBuffer() : IdxBuf(nullptr) {}
	~NTIndexBuffer()
	{
		if (nullptr != IdxBuf)
		{
			IdxBuf->Release();
		}
	}
};

class NTMesh : public NTResource
{
private:
	std::vector<UINT> BufSizeVec;
	std::vector<ID3D11Buffer*> VtxBufVec;
	std::vector<Autoptr<NTVertexBuffer>> VtxBufInfoVec;
	std::vector<Autoptr<NTIndexBuffer>> IdxBufVec;

	D3D11_PRIMITIVE_TOPOLOGY DrawMode;

public:
	bool Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize,
		D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

public:
	void SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY _DrawMode) { DrawMode = _DrawMode; }
	bool CreateVertex(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem);
	bool CreateIndex(UINT _TriangleCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, DXGI_FORMAT _Format, void* _IndexMem);

public:
	void Update(UINT _StartVtx = 0, UINT _VtxCount = 1, UINT* _Off = nullptr);
	void Render(UINT _StartIdx = 0, UINT _EndIdx = 1, UINT* _Off = nullptr);

	void UpdateAndRender(UINT _Vtx, UINT _Sub);

public:
	NTMesh();
	~NTMesh();
};
