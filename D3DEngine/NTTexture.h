#pragma once
#include "NTResource.h"
#include "DirectXTex.h"
#include "DirectHeader.h"

#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "DirectXTex_D86")
	#else
		#pragma comment(lib, "DirectXTex_D64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "DirectXTex_R86")
	#else
		#pragma comment(lib, "DirectXTex_R64")
	#endif
#endif


class NTTexture : public NTResource
{
private:
	DirectX::ScratchImage Image;
	ID3D11Texture2D* Tex2D;
	ID3D11ShaderResourceView* View;

public:
	float GetWidth()
	{
		return (float)Image.GetMetadata().width;
	}

	float GetHeight()
	{
		return (float)Image.GetMetadata().height;
	}

	NTVEC2 GetImageSize()
	{
		return{ GetWidth(), GetHeight() };
	}

	NTCOLOR GetPixel(int _X, int _Y);

public:
	bool Load();

public:
	void Update(unsigned int _Slot);

public:
	NTTexture();
	~NTTexture();
};

