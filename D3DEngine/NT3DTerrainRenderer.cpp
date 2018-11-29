#include "PreCom.h"
#include "NT3DTerrainRenderer.h"
#include "NTMesh.h"
#include "DirectValueHeader.h"

NT3DTerrainRenderer::NT3DTerrainRenderer()
{
}


NT3DTerrainRenderer::~NT3DTerrainRenderer()
{
}

void NT3DTerrainRenderer::CreateTerrain(unsigned int _SizeX, unsigned int _SizeZ)
{
	Data.TexCount = 0;
	Data.SizeX = _SizeX;
	Data.SizeZ = _SizeZ;
	for (int i = 0; i < 4; i++)
	{
		Data.IsBump[i] = 0;
	}

	if (Data.SizeX <= 0 || Data.SizeZ <=0)
	{
		tassert(true);
		return;
	}

	Autoptr<NTMesh> Mesh = new NTMesh;

	std::vector<Vtx3D> Vtx; 
	std::vector<DWORD> Idx;

	Vtx3D Tmp;

	for (int z = 0; z < Data.SizeZ + 1; ++z)
	{
		for (int x = 0; x < Data.SizeX + 1; ++x)
		{
			Tmp.Pos = NTVEC((float)x, 0.0f, (float)z, 1.0f);
			Tmp.Uv = NTVEC2((float)x, float(Data.SizeZ - z));
			Tmp.Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
			Tmp.Normal = NTVEC(0.0f, 1.0f, 0.0f, 0.0f);
			Tmp.Tangent = NTVEC(1.0f, 0.0f, 0.0f, 0.0f);
			Tmp.BiNormal = NTVEC(0.0f, 0.0f, -1.0f, 0.0f);

			Vtx.push_back(Tmp);
		}
	}

	for (int z = 0; z < Data.SizeZ; ++z)
	{
		for (int x = 0; x < Data.SizeX; ++x)
		{
			Idx.push_back((Data.SizeX + 1) * (z + 1) + (x));
			Idx.push_back((Data.SizeX + 1) * (z ) + (x + 1));
			Idx.push_back((Data.SizeX + 1) * (z) + (x));

			Idx.push_back((Data.SizeX + 1) * (z) + (x + 1));
			Idx.push_back((Data.SizeX + 1) * (z + 1) + (x));
			Idx.push_back((Data.SizeX + 1) * (z + 1) + (x + 1));
		}
	}

	Mesh->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mesh->CreateVertex((UINT)Vtx.size(), (UINT)Vtx3D::TypeSize(), D3D11_USAGE_DYNAMIC, &Vtx[0]);
	Mesh->CreateIndex((UINT)Idx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::GetFormat(), &Idx[0]);

	SetMesh(Mesh);

	SetMaterial(L"TerrainDefferdMat");
}

void NT3DTerrainRenderer::Render(Autoptr<NTCamera> _Camera)
{
	GetMaterial()->GetPixelShader()->SetConstBuffer<TerrainFloorData>(L"TerrainFloorData", Data);
}

void NT3DTerrainRenderer::SetBaseTexture(const wchar_t * _MultiTex)
{
	GetMaterial()->AddTextureData(TEXTYPE::TT_MULTI, Data.TexCount + 8, _MultiTex, Data.TexCount + 8, L"TerrainSmp");
	++Data.TexCount;
	// 멀티텍스처 세팅할때는 슬롯에 8을 더해줄것.
}

void NT3DTerrainRenderer::AddSplattingTexture(const wchar_t * _MultiTex, const wchar_t * _SplattingTex)
{
	if (Data.TexCount <= 0)
	{
		tassert(true);
		return;
	}

	GetMaterial()->AddTextureData(TEXTYPE::TT_COLOR, Data.FloorCount, _SplattingTex, Data.FloorCount);
	GetMaterial()->AddTextureData(TEXTYPE::TT_MULTI, Data.TexCount + 8, _MultiTex, Data.TexCount + 8, L"TerrainSmp");

	++Data.FloorCount;
	++Data.TexCount;
}
