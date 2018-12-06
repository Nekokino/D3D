#pragma once
#include "NTRenderer.h"

class TerrainFloorData
{
public:
	int FloorCount;
	int SizeX;
	int SizeZ;
	int TexCount;
	int IsBump[4];

public:
	TerrainFloorData() : FloorCount(0), SizeX(0), SizeZ(0), TexCount(0)
	{

	}
};

class NT3DTerrainRenderer : public NTRenderer
{
private:
	TerrainFloorData Data;
	std::vector<NTVEC> VtxVec;

public:
	void CreateTerrain(unsigned int _SizeX, unsigned int _SizeY, const wchar_t* _HeightMap = nullptr, float _HeighRatio = 1.0f);
	void Render(Autoptr<NTCamera> _Camera) override;

	void SetBaseTexture(const wchar_t* _MultiTex);
	void AddSplattingTexture(const wchar_t* _MultiTex, const wchar_t* _SplattingTex);

	float GetTerY(NTVEC _Pos);

public:
	NT3DTerrainRenderer();
	~NT3DTerrainRenderer();
};

