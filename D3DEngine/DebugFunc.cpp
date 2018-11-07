#include "PreCom.h"
#include "DebugFunc.h"
#include "NTWinShortCut.h"
#include "NTWindow.h"
#include "NTMesh.h"
#include "NTMaterial.h"
#include "ResourceSystem.h"
#include "NTFont.h"
#include "NTWinShortCut.h"
#include "NTSampler.h"
#include "NTMultiRenderTarget.h"

bool DebugFunc::bIsDebug = true;
float DebugFunc::LogSize = 20.0f;
std::list<DebugFunc::DbgLog>::iterator DebugFunc::ListStartIter;
std::list<DebugFunc::DbgLog>::iterator DebugFunc::ListEndIter;
std::list<DebugFunc::DbgLog> DebugFunc::Log;


DebugFunc::DebugFunc()
{
}


DebugFunc::~DebugFunc()
{
}


void DebugFunc::Init()
{
}

void DebugFunc::DrawRect(const NTRECT & _Rect, float _Border)
{
	NTVEC2 UvSize;
	UvSize.x = 1 / _Rect.Size.x;
	UvSize.y = 1 / _Rect.Size.y;

	UvSize *= _Border;

	NTMAT VP = NTWinShortCut::GetMainSceneSystem().GetCurScene()->GetMainCamera()->GetVP();

	
	NTMAT Scale;
	Scale.Scale(NTVEC(_Rect.Size.x, _Rect.Size.y, 1.0f));

	NTMAT Pos;
	Pos.Translate(NTVEC(_Rect.Pos.x, _Rect.Pos.y, 0.1f));

	Autoptr<NTMesh> Mesh = ResourceSystem<NTMesh>::Find(L"TEXMESH");
	Autoptr<NTMaterial> Material = ResourceSystem<NTMaterial>::Find(L"DbgRectMat");

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}

void DebugFunc::DrawCircle(const NTCIRCLE & _Circle, float _Border)
{
	float UvSize = 1 / _Circle.Diameter();

	UvSize *= _Border;

	NTMAT VP = NTWinShortCut::GetMainSceneSystem().GetCurScene()->GetMainCamera()->GetVP();

	NTMAT View;
	NTMAT Proj;
	View.ViewToLH(NTVEC::ZERO, NTVEC::FORWARD, NTVEC::UP);
	Proj.OrthographicLH(NTWinShortCut::GetMainWindow().GetWidthf(), NTWinShortCut::GetMainWindow().GetHeightf(), 0.1f, 1000.0f);

	NTMAT Scale;
	Scale.Scale(NTVEC(_Circle.Radius, _Circle.Radius, 1.0f));

	NTMAT Pos;
	Pos.Translate(NTVEC(_Circle.Pos.x, _Circle.Pos.y, 0.1f));


	Autoptr<NTMesh> Mesh = ResourceSystem<NTMesh>::Find(L"LINECIRCLEMESH2");
	Autoptr<NTMaterial> Material = ResourceSystem<NTMaterial>::Find(L"CMAT");
	Material->GetVertexShader()->SetConstBuffer<NTMAT>(L"TRANS", (Scale * Pos * VP).RTranspose());

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}

void DebugFunc::DrawFont(wchar_t * _Str, NTVEC2 _Pos, float _Size, UINT _Color, FW1_TEXT_FLAG _Flag)
{
	Autoptr<NTFont> Find = ResourceSystem<NTFont>::Find(L"±Ã¼­");

	if (nullptr != Find)
	{
		Find->DrawFont(_Str, _Pos, _Size, _Color, _Flag);
	}

	NTWinShortCut::GetMainDevice().ResetContext();
}

void DebugFunc::DrawLog(wchar_t * _Str, NTVEC _Color)
{
	Log.push_back({ _Str , _Color });
}

void DebugFunc::RenderLog()
{
	ListStartIter = Log.begin();
	ListEndIter = Log.end();

	NTVEC2 Pos = { 10.0f, 10.0f };

	for (; ListStartIter != ListEndIter; ++ListStartIter)
	{
		DrawFont((wchar_t*)(*ListStartIter).Msg.c_str(), Pos, LogSize, (*ListStartIter).Color.ColorUINT());
		Pos.y = LogSize;
	}

	Log.clear();
}

void DebugFunc::TargetDebug()
{
	Autoptr<NTSampler> Smp = ResourceSystem<NTSampler>::Find(L"DefaultSampler");

	if (nullptr == Smp)
	{
		tassert(true);
		return;
	}

	std::vector<Autoptr<NTMultiRenderTarget>> Vec = ResourceSystem<NTMultiRenderTarget>::GetAllRes();

	Autoptr<NTMesh> Mesh = ResourceSystem<NTMesh>::Find(L"Rect3DMesh");
	Autoptr<NTMaterial> Material = ResourceSystem<NTMaterial>::Find(L"TargetDebugMat");

	NTMAT View;
	NTMAT Proj;
	View.ViewToLH(NTVEC::ZERO, NTVEC::FORWARD, NTVEC::UP);
	Proj.OrthographicLH(NTWinShortCut::GetMainWindow().GetWidthf(), NTWinShortCut::GetMainWindow().GetHeightf(), 0.1f, 1000.0f);

	MatrixData MatData;

	int CountX = 0;
	int CountY = 0;
	int WCount = 5;

	float SizeX = NTWinShortCut::GetMainWindow().GetWidthf() / WCount;
	float SizeY = NTWinShortCut::GetMainWindow().GetHeightf() / WCount;

	for (size_t i = 0; i < Vec.size(); i++)
	{
		std::vector<Autoptr<NTRenderTarget>> TargetVec = Vec[i]->GetTargetTextureList();

		for (size_t j = 0; j < TargetVec.size(); j++)
		{
			NTMAT Scale;
			NTMAT Pos;

			Scale.Identify();
			Scale.Scale(NTVEC(SizeX, SizeY, 1.0f));
			Pos.Identify();
			Pos.Translate(NTVEC(-NTWinShortCut::GetMainWindow().GetWidthf() * 0.5f + (CountX * SizeX) + (SizeX * 0.5f), NTWinShortCut::GetMainWindow().GetHeightf() * 0.5f + -(CountY * SizeY) - (SizeY * 0.5f), 1.1f));

			NTMAT World = Scale * Pos;

			MatData.View = View;
			MatData.Projection = Proj;
			MatData.World = World;
			MatData.WV = World * View;
			MatData.WVP = World * View * Proj;
			MatData.TransposeAll();

			Smp->Update(0);

			if (nullptr == TargetVec[j]->GetTexture()->GetSRV())
			{
				tassert(true);
				return;
			}

			TargetVec[j]->GetTexture()->Update(0);

			NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_VS);

			Material->Update();
			Mesh->Update();
			Mesh->Render();

			TargetVec[j]->GetTexture()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % WCount))
			{
				++CountY;
				CountX = 0;
			}
		}
	}

	CountX = 0;
	CountY += 1;

	std::map<int, Autoptr<NTCamera>>::iterator CamStartIter = NTWinShortCut::GetMainSceneSystem().GetCurScene()->RenderSystem.CameraMap.begin();
	std::map<int, Autoptr<NTCamera>>::iterator CamEndIter = NTWinShortCut::GetMainSceneSystem().GetCurScene()->RenderSystem.CameraMap.end();

	for (; CamStartIter != CamEndIter; ++CamStartIter)
	{
		std::vector<Autoptr<NTRenderTarget>> TargetVec = CamStartIter->second->CamTarget->GetTargetTextureList();

		for (size_t j = 0; j < TargetVec.size(); j++)
		{
			NTMAT Scale;
			NTMAT Pos;

			Scale.Identify();
			Scale.Scale(NTVEC(SizeX, SizeY, 1.0f));
			Pos.Identify();
			Pos.Translate(NTVEC((-NTWinShortCut::GetMainWindow().GetWidthf() * 0.5f) + (CountX * SizeX) + (SizeX * 0.5f), NTWinShortCut::GetMainWindow().GetHeightf() * 0.5f + (-CountY * SizeY) - (SizeY * 0.5f), 1.1f));

			NTMAT World = Scale * Pos;

			MatData.View = View;
			MatData.Projection = Proj;
			MatData.World = World;
			MatData.WV = World * View;
			MatData.WVP = World * View * Proj;
			MatData.TransposeAll();

			Smp->Update(0);

			if (nullptr == TargetVec[j]->GetTexture()->GetSRV())
			{
				tassert(true);
				return;
			}

			TargetVec[j]->GetTexture()->Update(0);

			NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_VS);

			Material->Update();
			Mesh->Update();
			Mesh->Render();

			TargetVec[j]->GetTexture()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % WCount))
			{
				++CountY;
				CountX = 0;
			}
		}
	}
}
