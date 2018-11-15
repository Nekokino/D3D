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
#include "NTLight.h"
#include "NTRenderer.h"
#include "NTTexture.h"
#include "NTSampler.h"
#include "NTMultiRenderTarget.h"


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

	if (nullptr != DepthStencilView)
	{
		DepthStencilView->Release();
	}

	if (nullptr != DepthStencilTexture)
	{
		DepthStencilTexture->Release();
	}

	if (nullptr != TargetView)
	{
		TargetView->Release();
	}

	if (nullptr != SwapChain)
	{
		SwapChain->Release();
	}

	if (nullptr != Context)
	{
		Context->Release();
	}

	if (nullptr != Device)
	{
		Device->Release();
	}

	if (nullptr != BackBuffer)
	{
		BackBuffer->Release();
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

void NTDevice::OMSet()
{
	Context->OMSetRenderTargets(1, &TargetView, DepthStencilView);
	//Context->OMSetDepthStencilState(DepthStencilState, 1);
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
	BackBuffer = nullptr;
	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer))
	{
		return false;
	}

	if (S_OK != Device->CreateRenderTargetView(BackBuffer, 0, &TargetView))
	{
		return false;
	}

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

	Context->OMSetRenderTargets(1, &TargetView, DepthStencilView); // 렌더타겟뷰와, 뎁스스텐실 뷰를 OM단계에 전달

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



void NTDevice::ClearTarget() // 그리기전에 지우기
{

	Context->ClearRenderTargetView(TargetView, Color.s);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void NTDevice::Present() // 출력
{
	SwapChain->Present(0, 0);
}



Autoptr<NTDevice::RasterState> NTDevice::FindRasterState(const wchar_t * _Name)
{
	return MapFind<Autoptr<RasterState>>(RasterStateMap, _Name);
}

void NTDevice::ResetRasterState()
{
	DefaultRasterState->Update();
}

void NTDevice::SetDefaultRasterState(const wchar_t * _Name)
{
	Autoptr<RasterState> RS = MapFind<Autoptr<RasterState>>(RasterStateMap, _Name);

	if (nullptr == RS)
	{
		tassert(true);
		return;
	}

	if (nullptr == RS->State)
	{
		tassert(true);
		return;
	}

	DefaultRasterState = RS;
	DefaultRasterState->Update();

	return;
}

void NTDevice::CreateRasterState(const wchar_t * _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	RasterState* RS = new RasterState();
	RS->Create(Device, Context, _FillMode, _CullMode);
	RasterStateMap.insert(std::unordered_map<std::wstring, Autoptr<RasterState>>::value_type(_Name, RS));
}

void NTDevice::SetRasterState(const wchar_t * _Name)
{
	Autoptr<RasterState> RS = MapFind<Autoptr<RasterState>>(RasterStateMap, _Name);

	if (nullptr == RS)
	{
		tassert(true);
		return;
	}

	RS->Update();
}

bool NTDevice::DefaultInit()
{


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

	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTMAT>(L"TRANS", D3D11_USAGE_DYNAMIC, 0);
	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"MULCOLOR", D3D11_USAGE_DYNAMIC, 0);
	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"IMGUV", D3D11_USAGE_DYNAMIC, 1);
	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"OUTLINE", D3D11_USAGE_DYNAMIC, 2);
	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"OUTLINECOLOR", D3D11_USAGE_DYNAMIC, 3);
	//NTWinShortCut::GetMainDevice().CreateConstBuffer<NTVEC>(L"SECONDUV", D3D11_USAGE_DYNAMIC, 4);

	return true;
}

bool NTDevice::Default3DInit()
{
	CreateDefaultRenderTarget();
	Create3DDefault();
	Create3DMesh();
	Create3DMaterial();
	return true;
}

bool NTDevice::Create3DDefault()
{
	ResourceSystem<NTBlend>::Create(L"AlphaBlend");
	ResourceSystem<NTFont>::Create(L"궁서", L"궁서");
	ResourceSystem<NTSampler>::Create(L"DefaultSampler");

	NTWinShortCut::GetMainDevice().CreateConstBuffer<MatrixData>(L"MatData", D3D11_USAGE_DYNAMIC, 10);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<RenderOption>(L"RenderOption", D3D11_USAGE_DYNAMIC, 11);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTLight::LightCBData>(L"LightData", D3D11_USAGE_DYNAMIC, 12);
	NTWinShortCut::GetMainDevice().CreateConstBuffer<NTLight::LightData>(L"DefferdLightData", D3D11_USAGE_DYNAMIC, 12);

	NTWinShortCut::GetMainDevice().CreateRasterState(L"SNONE", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
	NTWinShortCut::GetMainDevice().CreateRasterState(L"SFRONT", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	NTWinShortCut::GetMainDevice().CreateRasterState(L"SBACK", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);

	NTWinShortCut::GetMainDevice().CreateRasterState(L"WNONE", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_NONE);
	NTWinShortCut::GetMainDevice().CreateRasterState(L"WFRONT", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	NTWinShortCut::GetMainDevice().CreateRasterState(L"WBACK", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK);

	NTWinShortCut::GetMainDevice().SetDefaultRasterState(L"SBACK");

	D3D11_DEPTH_STENCIL_DESC DepthState;

	DepthState.DepthEnable = TRUE;
	// MASK_ALL 뎁스비교함. || MASK_ZERO 안함.
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;

	DepthState.StencilEnable = FALSE;
	DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	D3D11_DEPTH_STENCILOP_DESC StencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	DepthState.FrontFace = StencilOp;
	DepthState.BackFace = StencilOp;

	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"Debug", DepthState);

	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"Basic", DepthState);

	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"Always", DepthState);

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"LightDepth", DepthState);

	/////////////////////////////////////////////////////////// 볼륨메쉬용 뎁스 스테이트

	// 볼륨메쉬 Back

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_GREATER;
	DepthState.StencilEnable = TRUE;

	D3D11_DEPTH_STENCILOP_DESC LightStencil;
	LightStencil.StencilFunc = D3D11_COMPARISON_ALWAYS;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;

	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"BackStencil", DepthState);

	// 볼륨메쉬 Front

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;

	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"FrontStencil", DepthState);

	// 볼륨메쉬 Pass

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_ZERO;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;

	NTWinShortCut::GetMainDevice().CreateDepthStencilState(L"PassStencil", DepthState);

	NTWinShortCut::GetMainDevice().SetDefaultDepthStencilState(L"Basic");

	////////////////////////// 블렌드 스테이트

	D3D11_BLEND_DESC Desc;

	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = true;


	///// 디퓨즈 블렌드
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	///// 스펙큘러 블렌드
	Desc.RenderTarget[1].BlendEnable = true;
	Desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	Desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[2] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[3] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[4] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[5] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[6] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[7] = D3D11_RENDER_TARGET_BLEND_DESC();
	
	ResourceSystem<NTBlend>::Create(L"LightOne", Desc);

	Desc.RenderTarget[0] = D3D11_RENDER_TARGET_BLEND_DESC();
	Desc.RenderTarget[0].RenderTargetWriteMask = 0;

	ResourceSystem<NTBlend>::Create(L"Volume", Desc);

	ResourceSystem<NTTexture>::Load(L"Texture", L"SkyBox.png");

	return true;
}

bool NTDevice::CreateDefaultRenderTarget()
{
	//ResourceSystem<NTRenderTarget>::Create(L"BackBuffer", BackBuffer, D3D11_BIND_RENDER_TARGET);
	//ResourceSystem<NTRenderTarget>::Create(L"Forward", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE_DEFAULT); // 아니 디폴트인자 썻는데 왜안돼냐 버그진짜;

	// 디퍼드용
	ResourceSystem<NTRenderTarget>::Create(L"Color_Diffuse", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT);
	ResourceSystem<NTRenderTarget>::Create(L"Position", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT);
	ResourceSystem<NTRenderTarget>::Create(L"Normal", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT);
	ResourceSystem<NTRenderTarget>::Create(L"Depth", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), NTCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT);

	Autoptr<NTMultiRenderTarget> DeffertMRT = ResourceSystem<NTMultiRenderTarget>::Create(L"Defferd", L"Color_Diffuse", L"Position", L"Normal", L"Depth");

	ResourceSystem<NTRenderTarget>::Create(L"Light_Diffuse", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), NTCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceSystem<NTRenderTarget>::Create(L"Light_Specular", NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), NTCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	Autoptr<NTMultiRenderTarget> LightMRT = ResourceSystem<NTMultiRenderTarget>::Create(L"Light", L"Light_Diffuse", L"Light_Specular");

	/*Autoptr<NTMultiRenderTarget> BackMRT = ResourceSystem<NTMultiRenderTarget>::Create(L"Forward", L"BackBuffer", L"Forward");
	BackMRT->CreateDepth(NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU());*/
	return true;
}

bool NTDevice::Create3DMesh()
{
	Vtx3D Vtx;

#pragma region 3DRectMesh
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

#pragma endregion

#pragma region 3DCubeMesh
	std::vector<Vtx3D> Cube;
	std::vector<WORD> CubeIdx;

	Vtx.Pos = NTVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	// 아랫면
	Vtx.Pos = NTVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);


	// 오른쪽
	Vtx.Pos = NTVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	// 왼쪽
	Vtx.Pos = NTVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	Cube.push_back(Vtx);

	// 앞
	Vtx.Pos = NTVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	Cube.push_back(Vtx);

	// 뒤
	Vtx.Pos = NTVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	Cube.push_back(Vtx);

	Vtx.Pos = NTVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.0f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = NTVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	Cube.push_back(Vtx);

	// 윗면 인덱스
	CubeIdx.push_back(0); CubeIdx.push_back(1); CubeIdx.push_back(2);
	CubeIdx.push_back(0); CubeIdx.push_back(2); CubeIdx.push_back(3);

	// 아랫면
	CubeIdx.push_back(6); CubeIdx.push_back(5); CubeIdx.push_back(4);
	CubeIdx.push_back(6); CubeIdx.push_back(4); CubeIdx.push_back(7);

	// 오른쪽
	CubeIdx.push_back(8); CubeIdx.push_back(9); CubeIdx.push_back(10);
	CubeIdx.push_back(8); CubeIdx.push_back(10); CubeIdx.push_back(11);

	// 왼쪽 면
	CubeIdx.push_back(13); CubeIdx.push_back(12); CubeIdx.push_back(15);
	CubeIdx.push_back(13); CubeIdx.push_back(15); CubeIdx.push_back(14);

	// 앞면
	CubeIdx.push_back(17); CubeIdx.push_back(16); CubeIdx.push_back(19);
	CubeIdx.push_back(17); CubeIdx.push_back(19); CubeIdx.push_back(18);

	// 뒷면
	CubeIdx.push_back(20); CubeIdx.push_back(21); CubeIdx.push_back(22);
	CubeIdx.push_back(20); CubeIdx.push_back(22); CubeIdx.push_back(23);

	ResourceSystem<NTMesh>::Create(L"Cube", (UINT)Cube.size(), (UINT)Vtx3D::TypeSize(), D3D11_USAGE_DYNAMIC, &Cube[0], (UINT)CubeIdx.size(), (UINT)IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, &CubeIdx[0], IDX16::GetFormat());


#pragma endregion

#pragma region SphereMesh
	std::vector<Vtx3D> SphereVtx;
	std::vector<UINT> SphereIdx;

	float Rad = 0.5f;

	Vtx.Pos = NTVEC{ 0.0f, Rad, 0.0f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.5f, 0.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = Vtx.Pos;
	Vtx.Normal.Normalize3D();
	Vtx.Normal.w = 0.0f;
	Vtx.Tangent = NTVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	Vtx.BiNormal = NTVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	SphereVtx.push_back(Vtx); // 위쪽 꼭대기점.

	UINT StackCount = 40; // 가로 분할
	UINT SliceCount = 40; // 세로 분할

	float yAngle = DirectX::XM_PI / (float)StackCount; // 반바퀴만 돌아도 구가 그려지기 때문에 2파이 아니고 파이
	float zAngle = DirectX::XM_2PI / (float)SliceCount; // 얘가 한바퀴 돌아야 그려지니까 얘는 2파이

	float yUvRatio = 1.0f / (float)SliceCount;
	float zUvRatio = 1.0f / (float)StackCount;

	for (UINT y = 1; y < StackCount; ++y)
	{
		float Phi = y * yAngle;
		for (UINT z = 0; z < SliceCount + 1; ++z)
		{
			float Theta = z * zAngle;
			Vtx.Pos = NTVEC{
				Rad * sinf(y * yAngle) * cosf(z * zAngle),
				Rad * cosf(y * yAngle),
				Rad * sinf(y * yAngle) * sinf(z * zAngle)
			};
			Vtx.Pos.w = 1.0f;
			Vtx.Uv = NTVEC2{ yUvRatio * z, zUvRatio * y };
			Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
			Vtx.Normal = Vtx.Pos;
			Vtx.Normal.Normalize3D();
			Vtx.Normal.w = 0.0f;

			Vtx.Tangent.x = -Rad * sinf(Phi) * sinf(Theta);
			Vtx.Tangent.y = 0.0f;
			Vtx.Tangent.z = Rad * sinf(Phi) * cosf(Theta);
			Vtx.Tangent.Normalize3D();
			Vtx.Tangent.w = 0.0f;

			Vtx.BiNormal = NTVEC::Cross3D(Vtx.Tangent, Vtx.Normal);
			Vtx.BiNormal.Normalize3D();
			Vtx.BiNormal.w = 0.0f;

			SphereVtx.push_back(Vtx);
		}
	}

	Vtx.Pos = NTVEC{ 0.0f, -Rad, 0.0f, 1.0f };
	Vtx.Uv = NTVEC2{ 0.5f, 1.0f };
	Vtx.Color = NTVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vtx.Normal = Vtx.Pos;
	Vtx.Normal.Normalize3D();
	Vtx.Normal.w = 0.0f;
	SphereVtx.push_back(Vtx); // 아래쪽 꼭대기점.

	for (UINT i = 0; i < SliceCount; ++i) // 위쪽 꼭대기점 인덱스 설정
	{
		SphereIdx.push_back(0);
		SphereIdx.push_back(i + 2);
		SphereIdx.push_back(i + 1);
	}

	for (UINT y = 0; y < StackCount - 2; ++y) // 양 끝 꼭대기점은 따로 그려주기 때문에 
	{
		for (UINT z = 0; z < SliceCount; ++z)
		{
			SphereIdx.push_back((SliceCount + 1) * y + z + 1);
			SphereIdx.push_back((SliceCount + 1) * (y + 1) + (z + 1) + 1);
			SphereIdx.push_back((SliceCount + 1) * (y + 1) + z + 1); // 수로는 이해가 안되면 그림을 그려보시오.

			SphereIdx.push_back((SliceCount + 1) * y + z + 1);
			SphereIdx.push_back((SliceCount + 1) * y + (z + 1) + 1);
			SphereIdx.push_back((SliceCount + 1) * (y + 1) + (z + 1) + 1); // 수로는 이해가 안되면 그림을 그려보시오.
		}
	}

	UINT BottomStartIdx = (UINT)SphereVtx.size() - 1;

	for (UINT i = 0; i < SliceCount; ++i)
	{
		SphereIdx.push_back(BottomStartIdx);
		SphereIdx.push_back(BottomStartIdx - (i + 2));
		SphereIdx.push_back(BottomStartIdx - (i + 1));
	}

	ResourceSystem<NTMesh>::Create(L"Sphere", (UINT)SphereVtx.size(), (UINT)Vtx3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SphereVtx[0], (UINT)SphereIdx.size(), (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0], IDX32::GetFormat());


#pragma endregion

	return true;
}

bool NTDevice::Create3DMaterial()
{
	////////////////////////////////////////////////////////////////// 디폴트 쉐이더 시작

	Autoptr<NTVertexShader> DefaultVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"DefaultVtx", L"Shader", L"Default.fx", "VS_Default");
	DefaultVtx->AddLayoutClose("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Autoptr<NTPixelShader> DefaultPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"DefaultPix", L"Shader", L"Default.fx", "PS_Default");

	Autoptr<NTMaterial> DefaultMat = ResourceSystem<NTMaterial>::Create(L"DefaultMat");
	DefaultMat->SetVertexShader(L"DefaultVtx");
	DefaultMat->SetPixelShader(L"DefaultPix");
	DefaultMat->SetBlend(L"AlphaBlend");

	////////////////////////////////////////////////////////////////// 디폴트 쉐이더 끝

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
	Grid3DPix->CreateConstBuffer<NTVEC>(L"GridData", D3D11_USAGE_DYNAMIC, 0);

	Autoptr<NTMaterial> Grid3DMat = ResourceSystem<NTMaterial>::Create(L"Grid3DMat");
	Grid3DMat->SetVertexShader(L"Grid3DVtx");
	Grid3DMat->SetPixelShader(L"Grid3DPix");
	Grid3DMat->SetBlend(L"AlphaBlend");

	////////////////////////////////////////////////////////////////// 그리드 끝

	////////////////////////////////////////////////////////////////// 스카이 바악스 시작

	Autoptr<NTVertexShader> SkyBoxVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"SkyBoxVtx", L"Shader", L"SkyBox.fx", "VS_SkyBox");
	SkyBoxVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SkyBoxVtx->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	SkyBoxVtx->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SkyBoxVtx->AddLayoutClose("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	Autoptr<NTPixelShader> SkyBoxPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"SkyBoxPix", L"Shader", L"SkyBox.fx", "PS_SkyBox");

	Autoptr<NTMaterial> SkyBoxMat = ResourceSystem<NTMaterial>::Create(L"SkyBoxMat");
	SkyBoxMat->SetVertexShader(L"SkyBoxVtx");
	SkyBoxMat->SetPixelShader(L"SkyBoxPix");
	SkyBoxMat->SetBlend(L"AlphaBlend");
	SkyBoxMat->AddTextureData(TEXTYPE::TT_COLOR, 0, L"SkyBox.png");

	///////////////////////////////////////////////////////////////// 스카이 바악스 끝

	Autoptr<NTVertexShader> Mesh3DVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"Mesh3DVtx", L"Shader", L"MeshMat.fx", "VS_Mesh3D");
	Mesh3DVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Mesh3DVtx->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	Mesh3DVtx->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Mesh3DVtx->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Mesh3DVtx->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	Mesh3DVtx->AddLayoutClose("BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	

	Autoptr<NTPixelShader> Mesh3DPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"Mesh3DPix", L"Shader", L"MeshMat.fx", "PS_Mesh3D");

	Autoptr<NTMaterial> Mesh3DMat = ResourceSystem<NTMaterial>::Create(L"Mesh3DMat");
	Mesh3DMat->SetVertexShader(L"Mesh3DVtx");
	Mesh3DMat->SetPixelShader(L"Mesh3DPix");
	Mesh3DMat->SetBlend(L"AlphaBlend");

	/////////////////////////////////////////////////////////////////// 디버그용

	Autoptr<NTVertexShader> TargetDebugVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"TargetDebugVtx", L"Shader", L"TargetDebug.fx", "VS_TDBG");
	TargetDebugVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TargetDebugVtx->AddLayoutClose("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);

	Autoptr<NTPixelShader> TargetDebugPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"TargetDebugPix", L"Shader", L"TargetDebug.fx", "PS_TDBG");

	Autoptr<NTMaterial> TargetDebugMat = ResourceSystem<NTMaterial>::Create(L"TargetDebugMat");
	TargetDebugMat->SetVertexShader(L"TargetDebugVtx");
	TargetDebugMat->SetPixelShader(L"TargetDebugPix");
	TargetDebugMat->SetBlend(L"AlphaBlend");

	/////////////////////////////////////////////////////////////////// 디버그용


	/////////////////////////////////////////////////////////////////// 디퍼드용

	Autoptr<NTVertexShader> DefferdVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"DefferdVtx", L"Shader", L"Defferd.fx", "VS_Defferd");
	DefferdVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdVtx->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	DefferdVtx->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdVtx->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdVtx->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdVtx->AddLayoutClose("BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	Autoptr<NTPixelShader> DefferdPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"DefferdPix", L"Shader", L"Defferd.fx", "PS_Defferd");

	Autoptr<NTMaterial> DefferdMat = ResourceSystem<NTMaterial>::Create(L"DefferdMat");
	DefferdMat->SetVertexShader(L"DefferdVtx");
	DefferdMat->SetPixelShader(L"DefferdPix");
	DefferdMat->SetBlend(L"AlphaBlend");
	
	///////////////////////////////////////////////////////////////////// 디퍼드용 끝

	///////////////////////////////////////////////////////////////////// 디퍼드 직선광용

	Autoptr<NTVertexShader> DefferdLightVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"DeffedLightVtx", L"Shader", L"Defferd.fx", "VS_DefferdLight");
	DefferdLightVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdLightVtx->AddLayoutClose("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);

	Autoptr<NTPixelShader> DefferdLightPix = ResourceSystem<NTPixelShader>::LoadFromKey(L"DefferdLightPix", L"Shader", L"Defferd.fx", "PS_DefferdLight");

	Autoptr<NTMaterial> DefferdLightMat = ResourceSystem<NTMaterial>::Create(L"DefferdLightMat");
	DefferdLightMat->SetVertexShader(L"DeffedLightVtx");
	DefferdLightMat->SetPixelShader(L"DefferdLightPix");
	DefferdLightMat->SetBlend(L"LightOne");

	DefferdLightMat->AddTextureData(TEXTYPE::TT_TARGET, 0, L"Position");
	DefferdLightMat->AddTextureData(TEXTYPE::TT_TARGET, 1, L"Normal");
	DefferdLightMat->AddTextureData(TEXTYPE::TT_TARGET, 2, L"Depth");

	///////////////////////////////////////////////////////////////////// 디퍼드 라이트용 끝

	///////////////////////////////////////////////////////////////////// 디퍼드 머지용

	Autoptr<NTVertexShader> DefferdMergeVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"DefferdMergeVtx", L"Shader", L"Defferd.fx", "VS_DefferdMerge");
	DefferdMergeVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DefferdMergeVtx->AddLayoutClose("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);

	Autoptr<NTPixelShader> DefferdMergePix = ResourceSystem<NTPixelShader>::LoadFromKey(L"DefferdMergePix", L"Shader", L"Defferd.fx", "PS_DefferdMerge");

	Autoptr<NTMaterial> DefferdMergeMat = ResourceSystem<NTMaterial>::Create(L"DefferdMergeMat");
	DefferdMergeMat->SetVertexShader(L"DefferdMergeVtx");
	DefferdMergeMat->SetPixelShader(L"DefferdMergePix");
	DefferdMergeMat->SetBlend(L"AlphaBlend");

	DefferdMergeMat->AddTextureData(TEXTYPE::TT_TARGET, 0, L"Color_Diffuse");
	DefferdMergeMat->AddTextureData(TEXTYPE::TT_TARGET, 1, L"Light_Diffuse");
	DefferdMergeMat->AddTextureData(TEXTYPE::TT_TARGET, 2, L"Light_Specular");

	///////////////////////////////////////////////////////////////////// 디퍼드 머지용 끝

	///////////////////////////////////////////////////////////////////// 화면 머지용

	Autoptr<NTVertexShader> ScreenMergeVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"ScreenMergeVtx", L"Shader", L"ScreenMerge.fx", "VS_ScreenMerge");
	ScreenMergeVtx->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ScreenMergeVtx->AddLayoutClose("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);

	Autoptr<NTPixelShader> ScreenMergePix = ResourceSystem<NTPixelShader>::LoadFromKey(L"ScreenMergePix", L"Shader", L"ScreenMerge.fx", "PS_ScreenMerge");

	Autoptr<NTMaterial> ScreenMergeMat = ResourceSystem<NTMaterial>::Create(L"ScreenMergeMat");
	ScreenMergeMat->SetVertexShader(L"ScreenMergeVtx");
	ScreenMergeMat->SetPixelShader(L"ScreenMergePix");

	///////////////////////////////////////////////////////////////////// 화면 머지용

	///////////////////////////////////////////////////////////////////// 볼륨메쉬

	Autoptr<NTVertexShader> VolumeVtx = ResourceSystem<NTVertexShader>::LoadFromKey(L"VolumeVtx", L"Shader", L"VolumeMesh.fx", "VS_Volume");
	VolumeVtx->AddLayoutClose("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	Autoptr<NTPixelShader> VolumePix = ResourceSystem<NTPixelShader>::LoadFromKey(L"VolumePix", L"Shader", L"VolumeMesh.fx", "PS_Volume");

	Autoptr<NTMaterial> VolumeMat = ResourceSystem<NTMaterial>::Create(L"VolumeMat");
	VolumeMat->SetVertexShader(L"VolumeVtx");
	VolumeMat->SetPixelShader(L"VolumePix");
	VolumeMat->SetBlend(L"Volume");

	///////////////////////////////////////////////////////////////////// 볼륨메쉬


	return true;
}

void NTDevice::RasterState::Update()
{
	Context->RSSetState(State);
}

void NTDevice::RasterState::Create(ID3D11Device * _Device, ID3D11DeviceContext * _Context, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	if (nullptr == _Context)
	{
		tassert(true);
		return;
	}

	Desc.FillMode = _FillMode;
	Desc.CullMode = _CullMode;
	Context = _Context;

	if (S_OK != _Device->CreateRasterizerState(&Desc, &State))
	{
		return;
	}
}

bool NTDevice::CreateConstBuffer(GlobalConstBuffer * _Buf)
{
	if (S_OK != Device->CreateBuffer(&_Buf->Desc, nullptr, &_Buf->CB))
	{
		delete _Buf;
		return false;
	}

	return true;
}

Autoptr<NTDevice::GlobalConstBuffer> NTDevice::FindConstBuffer(const wchar_t * _Name)
{
	return MapFind<Autoptr<NTDevice::GlobalConstBuffer>>(ConstBufferMap, _Name);
}


void NTDevice::DepthStencilState::Update(unsigned int _Ref)
{
	Context->OMSetDepthStencilState(DSS, _Ref);
}

void NTDevice::DepthStencilState::Create(ID3D11Device * _Device, ID3D11DeviceContext * _Context, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	if (nullptr == _Context)
	{
		tassert(true);
		return;
	}

	Context = _Context;

	if (S_OK != _Device->CreateDepthStencilState(&_Desc, &DSS))
	{
		return;
	}
}

Autoptr<NTDevice::DepthStencilState> NTDevice::FindDepthStencilState(const wchar_t * _Name)
{
	return MapFind<Autoptr<DepthStencilState>>(DepthStencilStateMap, _Name);
}


void NTDevice::ResetDepthStencilState()
{
	DefaultDepthStencilState->Update();
}

void NTDevice::SetDefaultDepthStencilState(const wchar_t* _Name)
{
	Autoptr<DepthStencilState> DSS = MapFind<Autoptr<DepthStencilState>>(DepthStencilStateMap, _Name);

	if (nullptr == DSS)
	{
		tassert(true);
		return;
	}

	if (nullptr == DSS->DSS)
	{
		tassert(true);
		return;
	}

	DefaultDepthStencilState = DSS;
	DefaultDepthStencilState->Update();
	return;
}

void NTDevice::CreateDepthStencilState(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	DepthStencilState* DSS = new DepthStencilState();
	DSS->Create(Device, Context, _Desc);
	DepthStencilStateMap.insert(std::unordered_map<std::wstring, Autoptr<DepthStencilState>>::value_type(_Name, DSS));
}

void NTDevice::SetDepthStencilState(const wchar_t* _Name, unsigned int _Ref)
{
	Autoptr<DepthStencilState> DSS = MapFind<Autoptr<DepthStencilState>>(DepthStencilStateMap, _Name);

	if (nullptr == DSS)
	{
		tassert(true);
		return;
	}

	DSS->Update(_Ref);
}

void NTDevice::SetBlendState(const wchar_t * _Name)
{
	Autoptr<NTBlend> Blend = ResourceSystem<NTBlend>::Find(_Name);

	if (Blend == nullptr)
	{
		tassert(true);
		return;
	}

	Blend->Update();
}
