#pragma once
#include "NTResource.h"


struct VertexTexture
{
	NTVEC Pos;
	NTVEC Uv;
};

struct VertexColor
{
	NTVEC Pos;
	NTVEC Color;
};

class NTMesh : public NTResource
{
private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	DXGI_FORMAT IndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY DrawMode;
	UINT VertexSize;
	UINT IndexCount;

public:
	bool Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize,
		D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	bool CreateVertex(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem);
	bool CreateIndex(UINT _TriangleCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem);

public:
	void Update();
	void Render();

public:
	NTMesh();
	~NTMesh();
};

