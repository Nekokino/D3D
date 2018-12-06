#pragma once
#include "DirectHeader.h"
#include "NTWinParent.h"
#include "RefNAutoptr.h"

#include <string>
#include <unordered_map>
#include <d3dcommon.h>

class NTDevice : public NTWinParent
{
public:
	friend class NTRenderSystem;
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

	ID3D11Texture2D* BackBuffer;
	ID3D11RenderTargetView* TargetView;

	ID3D11DepthStencilView* DepthStencilView;
	IDXGISwapChain* SwapChain;		// 스왑체인 알아두기.
	ID3D11Texture2D* DepthStencilTexture;
	UINT iMSLv;
	NTCOLOR Color;
	bool bInit;

	ID3D11Debug* GDebug = nullptr;
	ID3D11InfoQueue* InfoQueue = nullptr;
	

	/*D3D11_DEPTH_STENCIL_DESC DepthState;
	ID3D11DepthStencilState* DepthStencilState;
	ID3D11DepthStencilState* DepthStencilStateDebug;*/

public:
	ID3D11Device* GetDevice() { return Device; }
	ID3D11DeviceContext* GetContext() { return Context; }
	ID3D11DepthStencilView* GetDepth() { return DepthStencilView; }

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
	Autoptr<RasterState> DefaultRasterState;
	std::unordered_map<std::wstring, Autoptr<RasterState>> RasterStateMap;

private:
	Autoptr<RasterState> FindRasterState(const wchar_t* _Name);

public:
	void ResetRasterState();
	void SetDefaultRasterState(const wchar_t* _Name);
	void CreateRasterState(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);
	void SetRasterState(const wchar_t* _Name);

private:
	class DepthStencilState : public RefCounter
	{
	public:
		ID3D11DeviceContext* Context;
		D3D11_DEPTH_STENCIL_DESC Desc;
		ID3D11DepthStencilState* DSS;

	public:
		void Update(unsigned int _Ref = 0);
		void Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context, D3D11_DEPTH_STENCIL_DESC _Desc);

	public:
		~DepthStencilState()
		{
			if (nullptr != DSS)
			{
				DSS->Release();
			}
		}
	};

private:
	Autoptr<DepthStencilState> DefaultDepthStencilState;
	std::unordered_map<std::wstring, Autoptr<DepthStencilState>> DepthStencilStateMap;

private:
	Autoptr<DepthStencilState> FindDepthStencilState(const wchar_t* _Name);

public:
	void ResetDepthStencilState();
	void SetDefaultDepthStencilState(const wchar_t* _Name);
	void CreateDepthStencilState(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc);

	void SetDepthStencilState(const wchar_t* _Name, unsigned int _Ref = 0);

public:
	void SetBlendState(const wchar_t* _Name);
	
public:
	bool CreateSwapChain();
	bool CreateView();
	bool CreateViewPort();

	bool DefaultInit();
	bool Default3DInit();

	bool Create3DDefault();
	bool CreateDefaultRenderTarget();
	bool Create3DMesh();
	bool Create3DMaterial();

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
	void OMSet();

	/////////////////////////////// ConstBuffer

private:
	class GlobalConstBuffer : public RefCounter
	{
	public:
		D3D11_BUFFER_DESC Desc;
		ID3D11Buffer* CB;
		UINT Reg;

		~GlobalConstBuffer()
		{
			if (nullptr != CB)
			{
				CB->Release();
			}
		}
	};

private:
	std::unordered_map<std::wstring, Autoptr<GlobalConstBuffer>> ConstBufferMap;

public:
	template<typename BufType>
	bool CreateConstBuffer(const wchar_t* _Name, D3D11_USAGE _Usage, UINT _Reg)
	{
		GlobalConstBuffer* Buf = new GlobalConstBuffer();

		Buf->Desc.ByteWidth = sizeof(BufType);
		Buf->Desc.Usage = _Usage;
		Buf->Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (Buf->Desc.Usage == D3D11_USAGE_DYNAMIC)
		{
			Buf->Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (false == CreateConstBuffer(Buf))
		{
			return false;
		}

		Buf->Reg = _Reg;

		ConstBufferMap.insert(std::unordered_map<std::wstring, Autoptr<GlobalConstBuffer>>::value_type(_Name, Buf));

		return true;
	}

	template<typename BufType>
	void SetConstBuffer(const wchar_t* _Name, const BufType& _Data, STYPE _Type)
	{
		Autoptr<GlobalConstBuffer> Buf = FindConstBuffer(_Name);

		if (nullptr == Buf || Buf->Desc.ByteWidth != sizeof(BufType))
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE Sub = {};
		Context->Map(Buf->CB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Sub);
		memcpy(Sub.pData, &_Data, sizeof(BufType));
		Context->Unmap(Buf->CB, 0);

		Context->VSSetConstantBuffers(Buf->Reg, 1, &Buf->CB);
		Context->PSSetConstantBuffers(Buf->Reg, 1, &Buf->CB);
		Context->DSSetConstantBuffers(Buf->Reg, 1, &Buf->CB);
		Context->HSSetConstantBuffers(Buf->Reg, 1, &Buf->CB);
		Context->GSSetConstantBuffers(Buf->Reg, 1, &Buf->CB);
	}

	bool CreateConstBuffer(GlobalConstBuffer* _Buf);
	Autoptr<GlobalConstBuffer> FindConstBuffer(const wchar_t* _Name);


public:
	NTDevice(NTWindow* _Win);
	~NTDevice();
};

