#include "PreCom.h"
#include "NTSpRenderer.h"
#include "NTWindow.h"
#include "ResourceSystem.h"
#include "NTBlend.h"


NTSpRenderer::NTSpRenderer(): Color(1.0f, 1.0f, 1.0f, 1.0f), ImgIndex(0), Image(nullptr)
{
}


NTSpRenderer::~NTSpRenderer() 
{
}

void NTSpRenderer::Render(Autoptr<NTCamera> _Camera)
{
	tassert(nullptr == Transform);
	if (nullptr == Transform)
	{
		return;
	}

	if (nullptr == Image)
	{
		return;
	}

	SubTranformUpdate();

	NTVEC3 UvSize;
	UvSize.x = 1 / GetWorldSubScale().x / Image->GetCutSize().x;
	UvSize.y = 1 / GetWorldSubScale().y / Image->GetCutSize().y;
	UvSize.z = GetSubWorldMat().v1.x / 100.0f;

	NTCOLOR OutlineColor = NTVEC(1.0f, 0.0f, 0.0f, 1.0f); // �ƿ����ο� ������ ���� ���� �ް� �����ߵ�. �׷� ������ �������̵� �ؾߵǳ� ����. ������.

	float SecondUV = 1.0f;

	MaterialVec[0]->Update();
	Image->GetSamp()->Update(0);
	Image->GetTex()->Update(0);
	MeshVec[0]->Update();
	MeshVec[0]->Render();
}

void NTSpRenderer::SetImage(const wchar_t * _ImageName)
{
	Image = ResourceSystem<NTImage>::Find(_ImageName);
	tassert(nullptr == Image);
}

void NTSpRenderer::SetOriginScale()
{
	if (nullptr == Image)
	{
		return;
	}

	NTVEC Scale = Image->GetTex()->GetImageSize();

	Scale.z = 1.0f;

	SetSubScale(Scale);
}

bool NTSpRenderer::Init(int _Order)
{

	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"IMGMAT"))
	{
		return false;
	}

	if (false == SetMesh(L"TEXMESH"))
	{
		return false;
	}

	return true;
}

bool NTSpRenderer::Init(const wchar_t* _ImgName, int _Order)
{
	SetImage(_ImgName);
	SetName(_ImgName);

	if (nullptr == Image)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	return true;
}