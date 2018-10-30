#include "NTFreeCamera.h"
#include <InputSystem.h>
#include <TimeSystem.h>
#include <DebugFunc.h>


NTFreeCamera::NTFreeCamera() : MoveSpeed(100.0f)
{
}


NTFreeCamera::~NTFreeCamera()
{
}

bool NTFreeCamera::Init()
{
	Camera = GetComponent<NTCamera>();
	Camera->SetPMode(NTCamera::PROJMODE::CPM_PERSPECTIVE);

#pragma region Create Key
	if (false == InputSystem::IsKey(L"FreeCamUp"))
	{
		InputSystem::CreateKey(L"FreeCamUp", 'W');
	}

	if (false == InputSystem::IsKey(L"FreeCamLeft"))
	{
		InputSystem::CreateKey(L"FreeCamLeft", 'A');
	}

	if (false == InputSystem::IsKey(L"FreeCamDown"))
	{
		InputSystem::CreateKey(L"FreeCamDown", 'S');
	}

	if (false == InputSystem::IsKey(L"FreeCamRight"))
	{
		InputSystem::CreateKey(L"FreeCamRight", 'D');
	}

	if (false == InputSystem::IsKey(L"FreeCamForward"))
	{
		InputSystem::CreateKey(L"FreeCamForward", 'Q');
	}

	if (false == InputSystem::IsKey(L"FreeCamBack"))
	{
		InputSystem::CreateKey(L"FreeCamBack", 'E');
	}

	if (false == InputSystem::IsKey(L"FreeCamRotKey"))
	{
		InputSystem::CreateKey(L"FreeCamRotKey", VK_RBUTTON);
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

void NTFreeCamera::MainUpdate()
{
	if (true == InputSystem::IsPressed(L"Boost"))
	{
		RotSpeed = 360.0f;
		MoveSpeed = 1000.0f;
	}
	else
	{
		RotSpeed = 90.0f;
		MoveSpeed = 100.0f;
	}

	if (true == InputSystem::IsDown(L"ModeChange"))
	{
		Camera->ChangePMode();
	}

	if (true == InputSystem::IsDown(L"ResetX"))
	{
		NTVEC Rot = Transform->GetLocalRotation();
		Rot.x = 0.0f;
		Transform->SetLocalRotation(Rot);
	}

	if (true == InputSystem::IsDown(L"ResetY"))
	{
		NTVEC Rot = Transform->GetLocalRotation();
		Rot.y = 0.0f;
		Transform->SetLocalRotation(Rot);
	}

	if (true == InputSystem::IsDown(L"ResetZ"))
	{
		NTVEC Rot = Transform->GetLocalRotation();
		Rot.z = 0.0f;
		Transform->SetLocalRotation(Rot);
	}

	if (true == InputSystem::IsDown(L"ResetAll"))
	{
		Transform->Reset();
		Transform->SetLocalMove(NTVEC(0.0f, 0.0f, -10.0f));
	}

	if (true == InputSystem::IsPressed(L"FreeCamUp"))
	{
		Transform->SetLocalMove(Transform->GetLocalUp() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamLeft"))
	{
		Transform->SetLocalMove(Transform->GetLocalLeft() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamDown"))
	{
		Transform->SetLocalMove(Transform->GetLocalDown() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamRight"))
	{
		Transform->SetLocalMove(Transform->GetLocalRight() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamForward"))
	{
		Transform->SetLocalMove(Transform->GetLocalForward() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamBack"))
	{
		Transform->SetLocalMove(Transform->GetLocalBack() * TimeSystem::DeltaTime() * MoveSpeed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamRotKey"))
	{
		Transform->SetLocalAccRotation(NTVEC(InputSystem::GetMouseDir().y * RotSpeed * TimeSystem::DeltaTime(), InputSystem::GetMouseDir().x * RotSpeed * TimeSystem::DeltaTime()));
	}
}

void NTFreeCamera::DbgRender()
{
	wchar_t Arr[256];

	swprintf_s(Arr, L"CameraPos %f, %f", Camera->GetNTObject()->GetTransform()->GetLocalPosition().x, Camera->GetNTObject()->GetTransform()->GetLocalPosition().y);
	DebugFunc::DrawFont(Arr, NTVEC{ 10.0f, 10.0f }, 20.0f, 0xff00ffff);
}