#include "PreCom.h"
#include "NTDevice.h"
#include "NTWindow.h"
#include "StlAssist.h"
#include "DirectValueHeader.h"
#include "ResourceSystem.h"
#include "NTMesh.h"
#include "NTMaterial.h"
#include "NTWinShortCut.h"
#include "NTBlend.h"
#include "NTFont.h"

#define CIRCLE 10

NTDevice::NTDevice(NTWindow* _Win) : NTWinParent(_Win), Device(nullptr), Context(nullptr), TargetView(nullptr), DepthStencilView(nullptr), DepthStencilTexture(nullptr), SwapChain(nullptr), bInit(false), Color(0.2f, 0.835f, 0.674f, 1.0f)
{
}


NTDevice::~NTDevice()
{
	Release();
}

void NTDevice::Release()
{
	for (size_t i = 0; i < RS_TYPE::MAX; i++)
	{
		if (nullptr != m_ArrRs[i])
		{
			m_ArrRs[i]->Release();
		}
	}

	if (nullptr != Device)
	{
		Device->Release();
	}

	if (nullptr != Context)
	{
		Context->Release();
	}

	if (nullptr != TargetView)
	{
		TargetView->Release();
	}

	if (nullptr != DepthStencilView)
	{
		DepthStencilView->Release();
	}

	if (nullptr != DepthStencilTexture)
	{
		DepthStencilTexture->Release();
	}

	if (nullptr != SwapChain)
	{
		SwapChain->Release();
	}

	if (nullptr != DepthStencilState)
	{
		DepthStencilState->Release();
	}
}

void NTDevice::ResetContext()
{
	Context->VSSetShader(nullptr, nullptr, 0);
	Context->HSSetShader(nullptr, nullptr, 0);
	Context->GSSetShader(nullptr, nullptr, 0);
	Context->CSSetShader(nullptr, nullptr, 0);
	Context->PSSetShader(nullptr, nullptr, 0);
}

void NTDevice::ResetDepthStencil()
{
	Context->OMSetRenderTargets(1, &TargetView, DepthStencilView);
	Context->OMSetDepthStencilState(DepthStencilState, 1);
}

bool NTDevice::Init()
{
	bInit = false;

	int Flag = 0;
#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, Flag, nullptr, 0, D3D11_SDK_VERSION, &Device, &eLv, &Context))
	{
		Release();
		return false;
	}

	Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iMSLv); // 멀티 샘플링 레벨 체크

	if (false == CreateSwapChain())
	{
		Release();
		return false;
	}

	if (false == CreateView())
	{
		Release();
		return false;
	}

	if (false == CreateViewPort())
	{
		Release();
		return false;
	}

	CreateRasterizerState();

	bInit = true;
	return true;
}

bool NTDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferDesc.Width = (UINT)GetNTWindow()->GetWidth();
	tDesc.BufferDesc.Height = (UINT)GetNTWindow()->GetHeight(); // 백버퍼 크기

	tDesc.BufferDesc.RefreshRate.Numerator = 60; // 분자
	tDesc.BufferDesc.RefreshRate.Denominator = 1; // 분모. 두개 합쳐서 초당 버퍼 갱신률.

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼 픽셀 정보 종류

	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 스캔라인 세팅. 여기서는 둘 다 알아서 해주세요.

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼 용도 설정 

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.OutputWindow = GetNTWindow()->GetHWND();

	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	tDesc.BufferCount = 1;

	tDesc.Windowed = GetNTWindow()->IsWindow(); // 전체화면? Yes -> Window

	IDXGIDevice* IdxDevice = nullptr;
	IDXGIAdapter* Adapter = nullptr;
	IDXGIFactory* Factory = nullptr;

	Device->QueryInterface(__uuidof(IdxDevice), (void**)&IdxDevice);
	if (nullptr == IdxDevice)
	{
		return false;
	}

	IdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&Adapter);
	if (nullptr == Adapter)
	{
		return false;
	}

	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&Factory);
	if (nullptr == Factory || S_OK != Factory->CreateSwapChain(Device, &tDesc, &SwapChain))
	{
		return false;
	}

	Factory->Release();
	Adapter->Release();
	IdxDevice->Release();

	return true;
}

bool NTDevice::CreateView() // 스왑체인에 사용할 텍스쳐 생성단계
{
	ID3D11Texture2D* BackBufferTexture = nullptr;
	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTexture))
	{
		return false;
	}

	if (S_OK != Device->CreateRenderTargetView(BackBufferTexture, 0, &TargetView))
	{
		return false;
	}

	BackBufferTexture->Release();

	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.Width = (UINT)GetNTWindow()->GetWidth();
	tDesc.Height = (UINT)GetNTWindow()->GetHeight();
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.MipLevels = 1; // 밉맵레벨

	tDesc.Usage = D3D11_USAGE_DEFAULT; // 기본 텍스쳐 용도
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 스텐실 버퍼용으로 사용

	Device->CreateTexture2D(&tDesc, 0, &DepthStencilTexture);

	if (nullptr == DepthStencilTexture)
	{
		return false;
	}

	if (S_OK != Device->CreateDepthStencilView(DepthStencilTexture, 0, &DepthStencilView))
	{
		return false;
	}

	DepthState.DepthEnable = TRUE;
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	DepthState.StencilEnable = FALSE;
	DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	const D3D11_DEPTH_STENCILOP_DESC DefaultStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	DepthState.FrontFace = DefaultStencilOp;
	DepthState.BackFace = DefaultStencilOp;

	Device->CreateDepthStencilState(&DepthState, &DepthStencilState);

	if (nullptr == DepthStencilState)
	{
		tassert(true);
	}

	Context->OMSetRenderTargets(1, &TargetView, DepthStencilView); // 렌더타겟뷰와, 뎁스스텐실 뷰를 OM단계에 전달
	Context->OMSetDepthStencilState(DepthStencilState, 1);

	return true;
}

bool NTDevice::CreateViewPort()
{
	D3D11_VIEWPORT tDesc = {};

	tDesc.TopLeftX = 0;
	tDesc.TopLeftY = 0;

	tDesc.Width = (float)GetNTWindow()->GetWidth();
	tDesc.Height = (float)GetNTWindow()->GetHeight();

	tDesc.MinDepth = 0;
	tDesc.MaxDepth = 1;

	Context->RSSetViewports(1, &tDesc);

	return true;
}

bool NTDevice::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC tDesc = {};

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	if (S_OK != Device->CreateRasterizerState(&tDesc, &m_ArrRs[(UINT)RS_TYPE::RT_FRONT]))
	{
		return false;
	}

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	if (S_OK != Device->CreateRasterizerState(&tDesc, &m_ArrRs[(UINT)RS_TYPE::RT_BACK]))
	{
		return false;
	}

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	if (S_OK != Device->CreateRasterizerState(&tDesc, &m_ArrRs[(UINT)RS_TYPE::RT_NONE]))
	{
		return false;
	}

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	tDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	if (S_OK != Device->CreateRasterizerState(&tDesc, &m_ArrRs[(UINT)RS_TYPE::RT_WIREFRAME]))
	{
		return false;
	}

	Context->RSSetState(m_ArrRs[(UINT)RS_TYPE::RT_NONE]);

	return true;
}

void NTDevice::ClearTarget() // 그리기전에 지우기
{

	Context->ClearRenderTargetView(TargetView, Color.s);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void NTDevice::Present() // 출력
{
	SwapChain->Present(0, 0);
}

Autoptr<NTDevice::NTConstBuffer> NTDevice::FindConstBuffer(const wchar_t* _Name)
{
	return MapFind<Autoptr<NTDevice::NTConstBuffer>>(ConstBufferMap, _Name);
}

bool NTDevice::DefaultInit()
{
	ResourceSystem<NTBlend>::Create(L"AlphaBlend");
	ResourceSystem<NTFont>::Create(L"궁서", L"궁서");


#pragma region COLVTX
	// 컬러버텍스

	ColVtx ArrColVtx[4] = {};
	IDX16 ArrColIDX[2] = {};
	ArrColVtx[0].Pos = NTVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrColVtx[1].Pos = NTVEC(0.5f, 0.5f, 0.0f, 1.0f);
	ArrColVtx[2].Pos = NTVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrColVtx[3].Pos = NTVEC(0.5f, -0.5f, 0.0f, 1.0f);

	ArrColVtx[0].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColVtx[1].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColVtx[2].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColVtx[3].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);

	ArrColIDX[0] = IDX16(0, 3, 2);
	ArrColIDX[1] = IDX16(0, 1, 3);
#pragma endregion

#pragma region CIRCLETEXVTX
	TexVtx ArrCircleTexVtx[37] = {};
	IDX16 ArrCircleTexIDX[36] = {};

	ArrCircleTexVtx[0].Pos = NTVEC(0.0f, 0.0f, 0.0f, 1.0f);

	for (size_t i = 1; i < 37; ++i)
	{
		ArrCircleTexVtx[i].Pos = NTVEC(cosf((90.0f - 10.0f * i - 1) * MathSystem::D2R), sinf((90.0f - 10.0f * i - 1) * MathSystem::D2R), 0.0f, 1.0f);
	}

	ArrCircleTexVtx[0].Uv = NTVEC2(0.5f, 0.5f);

	for (size_t i = 1; i < 37; ++i)
	{
		ArrCircleTexVtx[i].Uv = NTVEC2(cosf((90.0f - 10.0f * (i - 1)) * MathSystem::D2R) * 0.5f + 0.5f, sinf((90.0f - 10.0f * (i - 1)) * MathSystem::D2R) * -0.5f + 0.5f);
	}

	for (size_t i = 0; i < 36; i++)
	{
		ArrCircleTexIDX[i] = IDX16{ 0, WORD(i + 1), WORD(i + 2) };
	}

	ArrCircleTexIDX[35] = IDX16{ 0, 36, 1 };
#pragma endregion

#pragma region TEXVTX
	TexVtx ArrTexVtx[4] = {};
	IDX16 ArrTexIDX[2] = {};
	ArrTexVtx[0].Pos = NTVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVtx[1].Pos = NTVEC(0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVtx[2].Pos = NTVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrTexVtx[3].Pos = NTVEC(0.5f, -0.5f, 0.0f, 1.0f);

	ArrTexVtx[0].Uv = NTVEC2(0.0f, 0.0f);
	ArrTexVtx[1].Uv = NTVEC2(1.0f, 0.0f);
	ArrTexVtx[2].Uv = NTVEC2(0.0f, 1.0f);
	ArrTexVtx[3].Uv = NTVEC2(1.0f, 1.0f);

	ArrTexIDX[0] = IDX16(0, 3, 2);
	ArrTexIDX[1] = IDX16(0, 1, 3);
#pragma endregion

	//UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
	//	UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem,
	//	DXGI_FORMAT	_IdxFm, D3D11_PRIMITIVE_TOPOLOGY _eDrawMode

	// D3D11_USAGE_DEFAULT GPU에서만 변경될수 있다.
	// D3D11_USAGE_IMMUTABLE GPU에서도 변경할수 없다. 사용만 가능.
	// D3D11_USAGE_DYNAMIC CPU GPU 다 가능.
	// D3D11_USAGE_STAGING GPU에있는 값을 CPU에서 불러올수도 있다.

#pragma region Color Material

	ResourceSystem<NTMesh>::Create(L"COLORMESH"
		, 4, (UINT)ColVtx::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColVtx
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColIDX
		, IDX16::GetFormat());

	Autoptr<NTVertexShader> Col = ResourceSystem<NTVertexShader>::LoadFromKey(L"VSCOLOR", L"Shader", L"ColorShader.fx", "VS_Color");

	Col->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Col->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Col->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PXCOLOR", L"Shader", L"ColorShader.fx", "PX_Color");

	Autoptr<NTMaterial> COLMAT = ResourceSystem<NTMaterial>::Create(L"COLMAT");
	COLMAT->SetVertexShader(L"VSCOLOR");
	COLMAT->SetPixelShader(L"PXCOLOR");

#pragma endregion

#pragma region DbgRect
	WORD ArrLineRectIDX[5] = { 0, 1, 3, 2, 0 };

	ResourceSystem<NTMesh>::Create(L"LINERECTMESH", 4, (UINT)ColVtx::TypeSize(), D3D11_USAGE_DEFAULT, ArrColVtx, 5, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrLineRectIDX, IDX16::GetFormat(), D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Autoptr<NTVertexShader> DbgRectVertex = ResourceSystem<NTVertexShader>::LoadFromKey(L"VS_DBGRECTCOLOR", L"Shader", L"DbgRectShader.fx", "VS_DbgRect");
	DbgRectVertex->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DbgRectVertex->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DbgRectVertex->CreateLayout();
	ResourceSystem<NTPixelShader>::LoadFromKey(L"PX_DBGRECTCOLOR", L"Shader", L"DbgRectShader.fx", "PX_DbgRect");

	Autoptr<NTMaterial> DbgRectMat = ResourceSystem<NTMaterial>::Create(L"DbgRectMat");
	DbgRectMat->SetVertexShader(L"VS_DBGRECTCOLOR");
	DbgRectMat->SetPixelShader(L"PX_DBGRECTCOLOR");

#pragma endregion

#pragma region DbgCircleMeshMK2
	ResourceSystem<NTMesh>::Create(L"LINECIRCLEMESH2", 37, (UINT)TexVtx::TypeSize(), D3D11_USAGE_DEFAULT, ArrCircleTexVtx, 108, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrCircleTexIDX, IDX16::GetFormat());

	Autoptr<NTVertexShader> CircleVertex = ResourceSystem<NTVertexShader>::LoadFromKey(L"VS_DBGCIRCLE", L"Shader", L"DbgCircleShader2.fx", "VS_DbgCircle2");
	CircleVertex->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	CircleVertex->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	CircleVertex->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PX_DBGCIRCLE", L"Shader", L"DbgCircleShader2.fx", "PX_DbgCircle2");

	Autoptr<NTMaterial> CMAT = ResourceSystem<NTMaterial>::Create(L"CMAT");
	CMAT->SetVertexShader(L"VS_DBGCIRCLE");
	CMAT->SetPixelShader(L"PX_DBGCIRCLE");
	CMAT->SetBlend(L"AlphaBlend");
#pragma endregion

#pragma region TEXMESH
	ResourceSystem<NTMesh>::Create(L"TEXMESH", 4, (UINT)TexVtx::TypeSize(), D3D11_USAGE_DYNAMIC, ArrTexVtx, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrTexIDX, IDX16::GetFormat());

	Autoptr<NTVertexShader> Tex = ResourceSystem<NTVertexShader>::LoadFromKey(L"VSTEXTURE", L"Shader", L"TextureShader.fx", "VS_Texture");

	Tex->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Tex->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	Tex->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PXTEXTURE", L"Shader", L"TextureShader.fx", "PX_Texture");

	Autoptr<NTMaterial> TEXMAT = ResourceSystem<NTMaterial>::Create(L"TEXMAT");
	TEXMAT->SetVertexShader(L"VSTEXTURE");
	TEXMAT->SetPixelShader(L"PXTEXTURE");
	TEXMAT->SetBlend(L"AlphaBlend");
#pragma endregion

#pragma region OUTLINE
	Autoptr<NTVertexShader> OutlineVS = ResourceSystem<NTVertexShader>::LoadFromKey(L"VS_OUTLINE", L"Shader", L"OutlineShader.fx", "VS_Outline");

	OutlineVS->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	OutlineVS->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	OutlineVS->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PX_OUTLINE", L"Shader", L"OutlineShader.fx", "PX_Outline");

	Autoptr<NTMaterial> OutlineMat = ResourceSystem<NTMaterial>::Create(L"OutlineMat");
	OutlineMat->SetVertexShader(L"VS_OUTLINE");
	OutlineMat->SetPixelShader(L"PX_OUTLINE");
	OutlineMat->SetBlend(L"AlphaBlend");
#pragma endregion


#pragma region ImageMaterial
	Autoptr<NTVertexShader> Image = ResourceSystem<NTVertexShader>::LoadFromKey(L"VSIMG", L"Shader", L"ImageShader.fx", "VS_Img");
	Image->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Image->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	Image->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PXIMG", L"Shader", L"ImageShader.fx", "PX_Img");

	Autoptr<NTMaterial> ImageMat = ResourceSystem<NTMaterial>::Create(L"IMGMAT");
	ImageMat->SetVertexShader(L"VSIMG");
	ImageMat->SetPixelShader(L"PXIMG");
	ImageMat->SetBlend(L"AlphaBlend");
#pragma endregion

#pragma region TextWindowMaterial
	Autoptr<NTVertexShader> TWVS = ResourceSystem<NTVertexShader>::LoadFromKey(L"VS_TEXTWINDOW", L"Shader", L"TextWindowShader.fx", "VS_TextWindow");
	TWVS->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TWVS->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	TWVS->CreateLayout();

	ResourceSystem<NTPixelShader>::LoadFromKey(L"PX_TEXTWINDOW", L"Shader", L"TextWindowShader.fx", "PX_TextWindow");

	Autoptr<NTMaterial> TWMaterial = ResourceSystem<NTMaterial>::Create(L"TWMAT");
	ImageMat->SetVertexShader(L"VS_TEXTWINDOW");
	ImageMat->SetPixelShader(L"PX_TEXTWINDOW");
	ImageMat->SetBlend(L"AlphaBlend");
#pragma endregion

	// 셰이더에 넘겨줄 상수버퍼 설정하는 곳

	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTMAT>(L"TRANS", D3D11_USAGE_DYNAMIC, 0);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"MULCOLOR", D3D11_USAGE_DYNAMIC, 0);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"IMGUV", D3D11_USAGE_DYNAMIC, 1);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"OUTLINE", D3D11_USAGE_DYNAMIC, 2);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"OUTLINECOLOR", D3D11_USAGE_DYNAMIC, 3);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"SECONDUV", D3D11_USAGE_DYNAMIC, 4);

	return true;
}

bool NTDevice::Default3DInit()
{
	NTWinShortCut::GetMainDevice().CreateConstBuffer<MatrixData>(L"MatData", D3D11_USAGE_DYNAMIC, 0);

	Vtx3D Arr3DVtx[4] = {};

	Arr3DVtx[0].Pos = NTVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	Arr3DVtx[1].Pos = NTVEC(0.5f, 0.5f, 0.0f, 1.0f);
	Arr3DVtx[2].Pos = NTVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	Arr3DVtx[3].Pos = NTVEC(0.5f, -0.5f, 0.0f, 1.0f);

	Arr3DVtx[0].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Arr3DVtx[1].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Arr3DVtx[2].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Arr3DVtx[3].Color = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);

	Arr3DVtx[0].Uv = NTVEC2(0.0f, 0.0f);
	Arr3DVtx[1].Uv = NTVEC2(1.0f, 0.0f);
	Arr3DVtx[2].Uv = NTVEC2(0.0f, 1.0f);
	Arr3DVtx[3].Uv = NTVEC2(1.0f, 1.0f);

	Arr3DVtx[0].Normal = NTVEC(0.0f, 0.0f, -1.0f, 1.0f);
	Arr3DVtx[1].Normal = NTVEC(0.0f, 0.0f, -1.0f, 1.0f);
	Arr3DVtx[2].Normal = NTVEC(0.0f, 0.0f, -1.0f, 1.0f);
	Arr3DVtx[3].Normal = NTVEC(0.0f, 0.0f, -1.0f, 1.0f);

	IDX16 ArrColorIdx[2] = {};

	ArrColorIdx[0] = IDX16(0, 3, 2);
	ArrColorIdx[1] = IDX16(0, 1, 3);

	ResourceSystem<NTMesh>::Create(L"Rect3DMesh", 4, (UINT)Vtx3D::TypeSize(), D3D11_USAGE_DYNAMIC, Arr3DVtx, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, ArrColorIdx, IDX16::GetFormat());

	Autoptr<NTVertexShader> Rect3DVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"Rect3DVtx", L"Shader", L"Rect3D.fx", "VS_Rect3D");
	Rect3DVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Rect3DVtx->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	Rect3DVtx->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Rect3DVtx->AddLayoutClose("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	Autoptr<NTPixelShader> Rect3DPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"Rect3DPix", L"Shader", L"Rect3D.fx", "PS_Rect3D");

	Autoptr<NTMaterial> Rect3DMat = ResourceSystem<NTMaterial>::Create(L"Rect3DMat");
	Rect3DMat->SetVertexShader(L"Rect3DVtx");
	Rect3DMat->SetPixelShader(L"Rect3DPix");
	Rect3DMat->SetBlend(L"AlphaBlend");

	////////////////////////////////////////////////////////////////// 그리드 시작

	Autoptr<NTVertexShader> Grid3DVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"Grid3DVtx", L"Shader", L"Grid3D.fx", "VS_Grid3D");
	Grid3DVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Grid3DVtx->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	Grid3DVtx->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Grid3DVtx->AddLayoutClose("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	Autoptr<NTPixelShader> Grid3DPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"Grid3DPix", L"Shader", L"Grid3D.fx", "PS_Grid3D");

	Autoptr<NTMaterial> Grid3DMat = ResourceSystem<NTMaterial>::Create(L"Grid3DMat");
	Grid3DMat->SetVertexShader(L"Grid3DVtx");
	Grid3DMat->SetPixelShader(L"Grid3DPix");
	Grid3DMat->SetBlend(L"AlphaBlend");

	////////////////////////////////////////////////////////////////// 그리드 끝

	return true;
}
