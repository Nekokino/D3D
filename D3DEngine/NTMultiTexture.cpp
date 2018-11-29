#include "PreCom.h"
#include "NTMultiTexture.h"
#include "NTWinShortCut.h"

NTMultiTexture::NTMultiTexture()
{
}


NTMultiTexture::~NTMultiTexture()
{
	if (nullptr != SRV)
	{
		SRV->Release();
	}

	if (nullptr != MultiTexture)
	{
		MultiTexture->Release();
	}
}

void NTMultiTexture::CreateMultiTexture(D3D11_USAGE _Usage)
{
	D3D11_TEXTURE2D_DESC Desc = {};

	for (size_t i = 0; i < TexVec.size(); i++)
	{
		if (nullptr == TexVec[i]->Tex2D)
		{
			tassert(true);
			return;
		}

		TexVec[i]->Tex2D->GetDesc(&Desc);

		Desc.BindFlags = 0;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		Desc.Usage = D3D11_USAGE_STAGING;

		D3D11_SUBRESOURCE_DATA Sub = {};
		Sub.pSysMem = TexVec[i]->Image.GetPixels();
		Sub.SysMemPitch = (UINT)TexVec[i]->Image.GetImages()->rowPitch;
		Sub.SysMemSlicePitch = (UINT)TexVec[i]->Image.GetImages()->slicePitch;

		ID3D11Texture2D* CopyTex = nullptr;
		if (S_OK != NTWinShortCut::GetDevice()->CreateTexture2D(&Desc, &Sub, &CopyTex))
		{
			tassert(true);
			return;
		}
		Tex2DVec.push_back(CopyTex);
	}

	TexVec[0]->Tex2D->GetDesc(&Desc);

	D3D11_TEXTURE2D_DESC ArrDesc = {};
	ArrDesc.Width = Desc.Width;
	ArrDesc.Height = Desc.Height;
	ArrDesc.MipLevels = Desc.MipLevels;
	ArrDesc.Format = Desc.Format;
	ArrDesc.ArraySize = (UINT)Tex2DVec.size();
	ArrDesc.Usage = _Usage;
	ArrDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ArrDesc.SampleDesc.Count = 1;
	ArrDesc.SampleDesc.Quality = 0;

	MultiTexture = nullptr;

	if (S_OK != NTWinShortCut::GetDevice()->CreateTexture2D(&ArrDesc, nullptr, &MultiTexture))
	{
		tassert(true);
		return;
	}

	for (UINT i = 0; i < TexVec.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE Sub = {};

		NTWinShortCut::GetContext()->Map(Tex2DVec[i], 0, D3D11_MAP_READ, 0, &Sub);

		NTWinShortCut::GetContext()->UpdateSubresource(MultiTexture, D3D11CalcSubresource(0, i, ArrDesc.MipLevels), NULL, Sub.pData, Sub.RowPitch, Sub.DepthPitch);

		NTWinShortCut::GetContext()->Unmap(Tex2DVec[i], 0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc = {};

	ViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	ViewDesc.Texture2DArray.ArraySize = (UINT)Tex2DVec.size();
	ViewDesc.Texture2DArray.FirstArraySlice = 0;
	ViewDesc.Texture2DArray.MipLevels = Desc.MipLevels;
	ViewDesc.Texture2DArray.MostDetailedMip = 0;

	if (S_OK != NTWinShortCut::GetDevice()->CreateShaderResourceView(MultiTexture, &ViewDesc, &SRV))
	{
		tassert(true);
		return;
	}

	if (S_OK != DirectX::CaptureTexture(NTWinShortCut::GetDevice(), NTWinShortCut::GetContext(), MultiTexture, Image))
	{
		tassert(true);
		return;
	}

	for (size_t i = 0; i < Tex2DVec.size(); i++)
	{
		Tex2DVec[i]->Release();
	}
}

bool NTMultiTexture::Create()
{
	return true;
}

void NTMultiTexture::Update(unsigned int _Slot)
{
	NTWinShortCut::GetContext()->VSSetShaderResources(_Slot, 1, &SRV);
	NTWinShortCut::GetContext()->PSSetShaderResources(_Slot, 1, &SRV);
}

void NTMultiTexture::Reset(unsigned int _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	NTWinShortCut::GetContext()->VSSetShaderResources(_Slot, 1, &Reset);
	NTWinShortCut::GetContext()->PSSetShaderResources(_Slot, 1, &Reset);
}