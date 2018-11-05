#include "PreCom.h"
#include "NTTexture.h"
#include "NTWinShortCut.h"


NTTexture::NTTexture()
{
}


NTTexture::~NTTexture()
{
	if (nullptr != Tex2D)
	{
		Tex2D->Release();
	}

	if (nullptr != View)
	{
		View->Release();
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

	if (S_OK != DirectX::CreateShaderResourceView(NTWinShortCut::GetDevice(), Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &View))
	{
		return false;
	}

	tassert(nullptr == View);

	View->GetResource((ID3D11Resource**)(&Tex2D));

	tassert(nullptr == Tex2D);

	return true;
}

void NTTexture::Update(unsigned int _Slot)
{
	NTWinShortCut::GetContext()->VSSetShaderResources(_Slot, 1, &View);
	NTWinShortCut::GetContext()->PSSetShaderResources(_Slot, 1, &View);
}