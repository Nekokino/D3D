#pragma once
#include "DirectHeader.h"
#include "NTWinParent.h"
#include "RefNAutoptr.h"
#include "NTShader.h"

#include <string>
#include <unordered_map>
#include <d3dcommon.h>

class NTDevice : public NTWinParent
{
public:
	friend class NTWindow;
	friend class NTRenderer;

public:
	enum RS_TYPE // 래스터라이즈단계에 쓰는 뭔가
	{
		RT_FRONT,
		RT_BACK,
		RT_NONE,
		RT_WIREFRAME,
		MAX,
	};

private:
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;	// 함수 실행 전에 미리 정해줘야할 것들인것 같다.
	ID3D11RenderTargetView* TargetView;
	ID3D11Texture2D* DepthStencilTexture;
	ID3D11DepthStencilView* DepthStencilView;
	IDXGISwapChain* SwapChain;		// 스왑체인 알아두기.
	UINT iMSLv;
	NTCOLOR Color;
	bool bInit;

	ID3D11Debug* GDebug = nullptr;
	ID3D11InfoQueue* InfoQueue = nullptr;
	

	D3D11_DEPTH_STENCIL_DESC DepthState;
	ID3D11DepthStencilState* DepthStencilState;

public:
	ID3D11Device* GetDevice() { return Device; }
	ID3D11DeviceContext* GetContext() { return Context; }

	void CallDeviceDebug() // 호출 시점의 살아있는 그래픽 오브젝트들을 표시해줌
	{
		GetDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&GDebug));
		GDebug->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&InfoQueue));

#ifdef _DEBUG
		InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
		D3D11_MESSAGE_ID Hide[] = { D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS, };
		D3D11_INFO_QUEUE_FILTER Filter; // 디버그용
		memset(&Filter, 0, sizeof(Filter));
		Filter.DenyList.NumIDs = _countof(Hide);
		Filter.DenyList.pIDList = Hide;
		InfoQueue->AddStorageFilterEntries(&Filter);

		GDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);

		InfoQueue->Release();
		GDebug->Release();
	}

private:
	class RasterState : public RefCounter
	{
	public:
		ID3D11DeviceContext* Context;
		D3D11_RASTERIZER_DESC Desc;
		ID3D11RasterizerState* State;

		~RasterState()
		{
			State->Release();
		}

	public:
		void Update();
		void Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);
	};

private:
	bool IsAllStateDefault;
	Autoptr<RasterState> DefaultState;
	std::unordered_map<std::wstring, Autoptr<RasterState>> RasterStateMap;

private:
	Autoptr<RasterState> FindRasterState(const wchar_t* _Name);

public:
	void ResetRasterState();
	void SetDefaultRasterState(const wchar_t* _Name);
	void CreateRasterState(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);

public:
	class NTConstBuffer : public RefCounter // 상수버퍼
	{
	public:
		D3D11_BUFFER_DESC tDesc;
		ID3D11Buffer* ConstBuffer;
		UINT iReg;

		~NTConstBuffer()
		{
			if (nullptr != ConstBuffer)
			{
				ConstBuffer->Release();
			}
		}
	};

private:
	std::unordered_map<std::wstring, Autoptr<NTConstBuffer>> ConstBufferMap;

public:
	template<typename T>
	bool CreateConstBuffer(const wchar_t* _Name, D3D11_USAGE _Usage, UINT _iReg)
	{
		NTConstBuffer* NewBuf = new NTConstBuffer();

		NewBuf->tDesc.ByteWidth = sizeof(T);
		NewBuf->tDesc.Usage = _Usage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (S_OK != Device->CreateBuffer(&NewBuf->tDesc, nullptr, &NewBuf->ConstBuffer))
		{
			delete NewBuf;
			return false;
		}

		NewBuf->iReg = _iReg;

		ConstBufferMap.insert(std::unordered_map<std::wstring, Autoptr<NTConstBuffer>>::value_type(_Name, NewBuf));

		return true;
	}

	template<typename T>
	void SetCBData(const wchar_t* _Name, const T& _Data, NTShader::STYPE _Stype)
	{
		Autoptr<NTConstBuffer> Buf = FindConstBuffer(_Name);

		if (nullptr == Buf && Buf->tDesc.ByteWidth != sizeof(T))
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE tSub = {};
		Context->Map(Buf->ConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(T));
		Context->Unmap(Buf->ConstBuffer, 0);
		

		switch (_Stype)
		{
		case NTShader::ST_NONE:
			tassert(true);
			break;
		case NTShader::ST_VS:
			Context->VSSetConstantBuffers(Buf->iReg, 1, &Buf->ConstBuffer);
			break;
		case NTShader::ST_PX:
			Context->PSSetConstantBuffers(Buf->iReg, 1, &Buf->ConstBuffer);
			break;
		default:
			break;
		}
	}

	Autoptr<NTConstBuffer> FindConstBuffer(const wchar_t* _Name);
	
public:
	bool CreateSwapChain();
	bool CreateView();
	bool CreateViewPort();

	bool DefaultInit();
	bool Default3DInit();

private:
	void ClearTarget();
	void Present();

public:
	bool Init();
	bool IsInit()
	{
		return bInit;
	}

public:
	void Release();
	void ResetContext();
	void ResetDepthStencil();

public:
	NTDevice(NTWindow* _Win);
	~NTDevice();
};

