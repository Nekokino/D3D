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

public:
	void CreateTerrain(unsigned int _SizeX, unsigned int _SizeY);
	void Render(Autoptr<NTCamera> _Camera) override;

	void SetBaseTexture(const wchar_t* _MultiTex);
	void AddSplattingTexture(const wchar_t* _MultiTex, const wchar_t* _SplattingTex);

public:
	NT3DTerrainRenderer();
	~NT3DTerrainRenderer();
};

