#include "PreCom.h"
#include "NTTexture.h"
#include "NTWinShortCut.h"


NTTexture::NTTexture() : Tex2D(nullptr), SRV(nullptr), RTV(nullptr), DSV(nullptr)
{
	SubData = D3D11_MAPPED_SUBRESOURCE();
}


NTTexture::~NTTexture()
{
	if (nullptr != Tex2D)
	{
		Tex2D->Release();
	}

	if (nullptr != SRV)
	{
		SRV->Release();
	}

	if (nullptr != RTV)
	{
		RTV->Release();
	}

	if (nullptr != DSV)
	{
		DSV->Release();
	}

}

NTCOLOR NTTexture::GetPixel(int _X, int _Y)
{
	size_t Size = DirectHelper::GetFormatSize(Image.GetMetadata().format);
	CustomColor Color;
	uint8_t* Tmp = Image.GetPixels();

	Tmp += ((Image.GetMetadata().width * _Y) + _X) * Size;

	memcpy_s(&Color, Size, Tmp, Size);

	NTCOLOR Return = { (float)Color.b, (float)Color.g, (float)Color.r, (float)Color.a };
	return Return;
}

void NTTexture::SetPixel(void * _Src, size_t _Size)
{
	size_t TexSize = DirectHelper::GetFormatSize(Image.GetMetadata().format) * Image.GetMetadata().width * Image.GetMetadata().height;

	if (TexSize < _Size)
	{
		return;
	}

	NTWinShortCut::GetContext()->Map(Tex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubData);

	memcpy_s(SubData.pData, TexSize, _Src, _Size);

	NTWinShortCut::GetContext()->Unmap(Tex2D, 0);
}

bool NTTexture::Load()
{
	if (true == IsExtension(L".DDs") || true == IsExtension(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(GetPath(), DirectX::DDS_FLAGS_NONE, nullptr, Image))
		{
			return false;
		}
	}
	else if (true == IsExtension(L".tga") || true == IsExtension(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(GetPath(), nullptr, Image))
		{
			return false;
		}
	}
	else
	{
		if (S_OK != DirectX::LoadFromWICFile(GetPath(), DirectX::WIC_FLAGS_NONE, nullptr, Image))
		{
			return false;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(NTWinShortCut::GetDevice(), Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &SRV))
	{
		return false;
	}

	tassert(nullptr == SRV);

	SRV->GetResource((ID3D11Resource**)(&Tex2D));

	tassert(nullptr == Tex2D);

	return true;
}

void NTTexture::Update(unsigned int _Slot)
{
	NTWinShortCut::GetContext()->VSSetShaderResources(_Slot, 1, &SRV);
	NTWinShortCut::GetContext()->PSSetShaderResources(_Slot, 1, &SRV);
}

void NTTexture::Reset(unsigned int _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	NTWinShortCut::GetContext()->VSSetShaderResources(_Slot, 1, &Reset);
	NTWinShortCut::GetContext()->PSSetShaderResources(_Slot, 1, &Reset);
}

void NTTexture::SetView(UINT _BindFlag)
{
	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != NTWinShortCut::GetDevice()->CreateDepthStencilView(Tex2D, 0, &DSV))
		{
			tassert(true);
			return;
		}
	}
	else
	{
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != NTWinShortCut::GetDevice()->CreateRenderTargetView(Tex2D, 0, &RTV))
			{
				tassert(true);
				return;
			}
		}

		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != NTWinShortCut::GetDevice()->CreateShaderResourceView(Tex2D, 0, &SRV))
			{
				tassert(true);
				return;
			}
		}
	}

	if (S_OK != DirectX::CaptureTexture(NTWinShortCut::GetDevice(), NTWinShortCut::GetContext(), Tex2D, Image))
	{
		tassert(true);
		return;
	}
}

bool NTTexture::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage)
{
	D3D11_TEXTURE2D_DESC Desc = {};

	Desc.Width = _W;
	Desc.Height = _H;
	Desc.ArraySize = 1;
	Desc.BindFlags = _BindFlag;
	Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Desc.CPUAccessFlags = 0;
	}

	Desc.Format = _Format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;

	if (S_OK != NTWinShortCut::GetDevice()->CreateTexture2D(&Desc, nullptr, &Tex2D))
	{
		tassert(true);
		return false;
	}

	SetView(_BindFlag);

	return true;
}

bool NTTexture::Create(ID3D11Texture2D * _Tex2D, UINT _BindFlag)
{
	return false;
}