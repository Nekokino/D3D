#include "NTTraceCamera.h"
#include <InputSystem.h>
#include <TimeSystem.h>
#include <NT3DTerrainRenderer.h>

NTTraceCamera::NTTraceCamera() : Speed(10.0f), Boost(10.0f)
{
	Pivot = NTVEC(0.0f, 10.0f, -80.0f, 1.0f);
}


NTTraceCamera::~NTTraceCamera()
{
}


void NTTraceCamera::SetTrace(Autoptr<NTTransform> _TraceTransform)
{
	TraceTransform = _TraceTransform;

	GetTransform()->SetLocalPosition(TraceTransform->GetLocalPosition() + Pivot);

}

bool NTTraceCamera::Init()
{
	Camera = GetComponent<NTCamera>();
	Camera->SetPMode(NTCamera::PROJMODE::CPM_PERSPECTIVE);

#pragma region Create Key
	if (false == InputSystem::IsKey(L"TraceCamUp"))
	{
		InputSystem::CreateKey(L"TraceCamUp", 'W');
	}

	if (false == InputSystem::IsKey(L"TraceCamLeft"))
	{
		InputSystem::CreateKey(L"TraceCamLeft", 'A');
	}

	if (false == InputSystem::IsKey(L"TraceCamDown"))
	{
		InputSystem::CreateKey(L"TraceCamDown", 'S');
	}

	if (false == InputSystem::IsKey(L"TraceCamRight"))
	{
		InputSystem::CreateKey(L"TraceCamRight", 'D');
	}

	if (false == InputSystem::IsKey(L"TraceCamForward"))
	{
		InputSystem::CreateKey(L"TraceCamForward", 'Q');
	}

	if (false == InputSystem::IsKey(L"TraceCamBack"))
	{
		InputSystem::CreateKey(L"TraceCamBack", 'E');
	}

	if (false == InputSystem::IsKey(L"TraceCamRotKey"))
	{
		InputSystem::CreateKey(L"TraceCamRotKey", VK_RBUTTON);
	}

	if (false == InputSystem::IsKey(L"Boost"))
	{
		InputSystem::CreateKey(L"Boost", VK_LSHIFT);
	}

	if (false == InputSystem::IsKey(L"ModeChange"))
	{
		InputSystem::CreateKey(L"ModeChange", 'R');
	}

	if (false == InputSystem::IsKey(L"ResetX"))
	{
		InputSystem::CreateKey(L"ResetX", 'Z');
	}

	if (false == InputSystem::IsKey(L"ResetY"))
	{
		InputSystem::CreateKey(L"ResetY", 'X');
	}

	if (false == InputSystem::IsKey(L"ResetZ"))
	{
		InputSystem::CreateKey(L"ResetZ", 'C');
	}

	if (false == InputSystem::IsKey(L"ResetAll"))
	{
		InputSystem::CreateKey(L"ResetAll", 'V');
	}

#pragma endregion
	return true;
}

void NTTraceCamera::MainUpdate()
{
	if (TraceTransform == nullptr)
	{
		return;
	}

	if (true == InputSystem::IsPressed(L"TraceCamLeft"))
	{
		TraceTransform->SetLocalMove(TraceTransform->GetLocalLeft() * TimeSystem::DeltaTime() * Speed * Boost);
		Transform->SetLocalMove(TraceTransform->GetLocalLeft() * TimeSystem::DeltaTime() * Speed * Boost);
	}

	if (true == InputSystem::IsPressed(L"TraceCamRight"))
	{
		TraceTransform->SetLocalMove(TraceTransform->GetLocalRight() * TimeSystem::DeltaTime() * Speed * Boost);
		Transform->SetLocalMove(TraceTransform->GetLocalRight() * TimeSystem::DeltaTime() * Speed * Boost);
	}

	if (true == InputSystem::IsPressed(L"TraceCamForward"))
	{
		TraceTransform->SetLocalMove(TraceTransform->GetLocalForward() * TimeSystem::DeltaTime() * Speed * Boost);
		Transform->SetLocalMove(TraceTransform->GetLocalForward() * TimeSystem::DeltaTime() * Speed * Boost);
	}

	if (true == InputSystem::IsPressed(L"TraceCamBack"))
	{
		TraceTransform->SetLocalMove(TraceTransform->GetLocalBack() * TimeSystem::DeltaTime() * Speed * Boost);
		Transform->SetLocalMove(TraceTransform->GetLocalBack() * TimeSystem::DeltaTime() * Speed * Boost);
	}

	float Y = 0.0f;

	if (nullptr != Terrain)
	{
		Y = Terrain->GetTerY(TraceTransform->GetLocalPosition());
	}

	NTVEC PosT = TraceTransform->GetLocalPosition();
	NTVEC Pos = Transform->GetLocalPosition();

	Pos.y = Y + Pivot.y;
	PosT.y = Y;

	Transform->SetLocalPosition(Pos);
	TraceTransform->SetLocalPosition(PosT);
}

void NTTraceCamera::DbgRender()
{
}