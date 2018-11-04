#pragma once
#include "NTResource.h"
#include <unordered_map>
#include "NTDevice.h"


class ConstBuffer;
class NTShader : public NTResource
{
protected:
	STYPE Type;
	ID3DBlob* Blob; // 쉐이더 코드를 컴파일했을때 나오는 바이트 배열
	ID3DBlob* ErrBlob; // 에러 바이트 배열
	ID3D11DeviceContext* Context;

public:
	STYPE GetSType()
	{
		return Type;
	}
private:
	virtual void Update() = 0;

protected:
	class ConstBuffer : public RefCounter
	{
	public:
		D3D11_BUFFER_DESC Desc;
		ID3D11Buffer* CB;
		UINT Reg;

		~ConstBuffer()
		{
			if (nullptr != CB)
			{
				CB->Release();
			}
		}
	};

public:
	void DefaultInit();

private:
	std::unordered_map<std::wstring, Autoptr<ConstBuffer>> ConstBufferMap;

public:
	template<typename BufType>
	bool CreateConstBuffer(const wchar_t* _Name, D3D11_USAGE _Usage, UINT _Reg)
	{
		ConstBuffer* Buf = new ConstBuffer();

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

		if (nullptr == Buf->CB)
		{
			delete Buf;
			return false;
		}

		Buf->Reg = _Reg;

		ConstBufferMap.insert(std::unordered_map<std::wstring, Autoptr<ConstBuffer>>::value_type(_Name, Buf));

		return true;
	}

	template<typename BufType>
	void SetConstBuffer(const wchar_t* _Name, const BufType& _Data)
	{
		Autoptr<ConstBuffer> Buf = FindConstBuffer(_Name);

		if (nullptr == Buf || Buf->Desc.ByteWidth != sizeof(BufType))
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE Sub = {};
		Context->Map(Buf->CB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Sub);
		memcpy(Sub.pData, &_Data, sizeof(BufType));
		Context->Unmap(Buf->CB, 0);

		UpdateConstBuffer(Buf);
	}

	bool CreateConstBuffer(ConstBuffer* _Buf);
	Autoptr<ConstBuffer> FindConstBuffer(const wchar_t* _Name);
private:
	virtual void UpdateConstBuffer(Autoptr<ConstBuffer> _Buf) = 0;

public:
	NTShader();
	~NTShader();
};

