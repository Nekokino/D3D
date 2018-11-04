#include "PreCom.h"
#include "NT3DMeshRenderer.h"
#include "NTWindow.h"
#include "NTMesh.h"
#include "ResourceSystem.h"


NT3DMeshRenderer::NT3DMeshRenderer()
{
}


NT3DMeshRenderer::~NT3DMeshRenderer()
{
}

void NT3DMeshRenderer::Render(Autoptr<NTCamera> _Camera)
{
	tassert(nullptr == Transform);

	if (nullptr == Transform)
	{
		return;
	}

	SubTranformUpdate();

	if (nullptr != Image)
	{
		Image->GetSamp()->Update();
		Image->GetTex()->Update();
	}

}

void NT3DMeshRenderer::SetImage(const wchar_t * _Name)
{
	Image = ResourceSystem<NTImage>::Find(_Name);

	if (nullptr == Image)
	{
		tassert(true);
	}
}

bool NT3DMeshRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	SetRasterState(L"SBACK");

	return true;
}