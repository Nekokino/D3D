#include "PreCom.h"
#include "NT3DTerrainRenderer.h"
#include "NTMesh.h"
#include "DirectValueHeader.h"
#include "NTTexture.h"
#include "ResourceSystem.h"
#include "DebugFunc.h"
#include "NTCamera.h"

NT3DTerrainRenderer::NT3DTerrainRenderer()
{
}


NT3DTerrainRenderer::~NT3DTerrainRenderer()
{
}

void NT3DTerrainRenderer::CreateTerrain(unsigned int _SizeX, unsigned int _SizeZ, const wchar_t * _HeightMap, float _HeightRatio)
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

	Autoptr<NTTexture> Tex = new NTTexture;

	if (nullptr != _HeightMap)
	{
		Tex = ResourceSystem<NTTexture>::Find(_HeightMap);

		if (nullptr == Tex)
		{
			tassert(true);
			return;
		}
	}

	for (int z = 0; z < Data.SizeZ + 1; ++z)
	{
		for (int x = 0; x < Data.SizeX + 1; ++x)
		{
			if (nullptr != Tex)
			{
				int RatioX = (int)(Tex->GetWidth() / Data.SizeX);
				int RatioZ = (int)(Tex->GetHeight() / Data.SizeZ);

				NTVEC Color = Tex->GetPixel2(x * RatioX, (int)Tex->GetHeight() - (z * RatioZ));

				Tmp.Pos = NTVEC((float)x, Color.x * _HeightRatio, (float)z, 1.0f);
				VtxVec.push_back(Tmp.Pos);
			}
			else
			{
				Tmp.Pos = NTVEC((float)x, 0.0f, (float)z, 1.0f);
				VtxVec.push_back(Tmp.Pos);
			}
			Tmp.Pos.w = 1.0f;
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

	Mesh->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	Mesh->CreateVertex((UINT)Vtx.size(), (UINT)Vtx3D::TypeSize(), D3D11_USAGE_DYNAMIC, &Vtx[0]);
	Mesh->CreateIndex((UINT)Idx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::GetFormat(), &Idx[0]);

	SetMesh(Mesh);

	SetMaterial(L"TerrainDefferdTesMat");
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

float NT3DTerrainRenderer::GetTerY(NTVEC _Pos)
{
	int X = (int)(_Pos.x / GetTransform()->GetLocalScale().x);
	int Z = (int)(_Pos.z / GetTransform()->GetLocalScale().z);

	wchar_t Arr[256] = {};
	swprintf_s(Arr, L"Trace : %d, %d", X, Z);
	DebugFunc::DrawFont(Arr, NTVEC2(0.0f, 0.0f), 50.0f, 0xffffffff);

	if (0 > X)
	{
		return 0.0f;
	}

	if (0 > Z)
	{
		return 0.0f;
	}

	if (Data.SizeX <= X)
	{
		return 0.0f;
	}

	if (Data.SizeZ <= Z)
	{
		return 0.0f;
	}

	NTVEC v0 = VtxVec[((Z + 1) * (Data.SizeX + 1)) + (X)];
	NTVEC v1 = VtxVec[((Z + 1) * (Data.SizeX + 1)) + (X + 1)];
	NTVEC v2 = VtxVec[((Z) * (Data.SizeX + 1)) + (X)];
	NTVEC v3 = VtxVec[((Z) * (Data.SizeX + 1)) + (X + 1)];

	float f0 = 0.0f;
	float f1 = 0.0f;

	NTVEC Check;
	Check.x = _Pos.x / GetTransform()->GetLocalScale().x;
	Check.z = _Pos.z / GetTransform()->GetLocalScale().z;

	DirectX::TriangleTests::Intersects(Check, NTVEC::UP, v2, v1, v0, f0);
	DirectX::TriangleTests::Intersects(Check, NTVEC::UP, v2, v3, v1, f1);

	swprintf_s(Arr, L"CheckPos : %f, %f", f0, f1);
	DebugFunc::DrawFont(Arr, NTVEC2(0.0f, 50.0f), 50.0f, 0xffffffff);

	float Return = 0.0f;

	if (0 != f1 || 0 != f0)
	{
		if (0 != f0)
		{
			Return = f0;
		}
		else if (0 != f1)
		{
			Return = f1;
		}
	}

	return Return * GetTransform()->GetLocalScale().y;
}
