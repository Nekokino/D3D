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

	ID3D11ShaderResourceView* SRV;
	ID3D11RenderTargetView* RTV;
	ID3D11DepthStencilView* DSV;

	D3D11_MAPPED_SUBRESOURCE SubData;

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

	void SetPixel(void* _Src, size_t _Size);

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV;
	}

	ID3D11RenderTargetView* GetRTV()
	{
		return RTV;
	}

	ID3D11DepthStencilView* GetDSV()
	{
		return DSV;
	}

public:
	void SetView(UINT _BindFlag);

	bool Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _Tex2D, UINT _BindFlag);

public:
	bool Load();

public:
	void Update(unsigned int _Slot);
	void Reset(unsigned int _Slot);

public:
	NTTexture();
	~NTTexture();
};