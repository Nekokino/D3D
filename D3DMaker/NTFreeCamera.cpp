#include "NTFreeCamera.h"
#include <InputSystem.h>
#include <TimeSystem.h>
#include <DebugFunc.h>


NTFreeCamera::NTFreeCamera() : Speed(100.0f)
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
#pragma endregion
	return true;
}

void NTFreeCamera::MainUpdate()
{
	if (true == InputSystem::IsPressed(L"FreeCamUp"))
	{
		Transform->SetLocalMove(Transform->GetLocalUp() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamLeft"))
	{
		Transform->SetLocalMove(Transform->GetLocalLeft() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamDown"))
	{
		Transform->SetLocalMove(Transform->GetLocalDown() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamRight"))
	{
		Transform->SetLocalMove(Transform->GetLocalRight() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamForward"))
	{
		Transform->SetLocalMove(Transform->GetLocalForward() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamBack"))
	{
		Transform->SetLocalMove(Transform->GetLocalBack() * TimeSystem::DeltaTime() * Speed);
	}

	if (true == InputSystem::IsPressed(L"FreeCamRotKey"))
	{
		NTVEC2 CurPoint = InputSystem::GetMousePos();
		Rotation = LastMPoint - CurPoint;

		if (Rotation.x != 0.0f || Rotation.y != 0.0f)
		{
			int a = 0;
		}

		Transform->SetLocalAccRotation(NTVEC(Rotation.x, Rotation.y));
		LastMPoint = CurPoint;
	}
	else if (true == InputSystem::IsIdle(L"FreeCamRotKey"))
	{
		LastMPoint = InputSystem::GetMousePos();
	}
}

void NTFreeCamera::DbgRender()
{
	wchar_t Arr[256];

	swprintf_s(Arr, L"CameraPos %f, %f", Camera->GetNTObject()->GetTransform()->GetLocalPosition().x, Camera->GetNTObject()->GetTransform()->GetLocalPosition().y);
	DebugFunc::DrawFont(Arr, NTVEC{ 10.0f, 10.0f }, 20.0f, 0xffffffff);
}