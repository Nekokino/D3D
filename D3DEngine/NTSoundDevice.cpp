#include "PreCom.h"
#include "NTSoundDevice.h"


FMOD::System* NTSoundDevice::SoundSystem = nullptr;

NTSoundDevice::StaticDestroyer NTSoundDevice::Destroyer;

NTSoundDevice::NTSoundDevice()
{
}


NTSoundDevice::~NTSoundDevice()
{
}

void NTSoundDevice::Init()
{
	FMOD::System_Create(&SoundSystem);

	tassert(nullptr == SoundSystem);

	if (nullptr == SoundSystem)
	{
		return;
	}

	FMOD_RESULT Result = SoundSystem->init(32, FMOD_DEFAULT, nullptr);

	tassert(FMOD_OK != Result);

	if (FMOD_OK != Result)
	{
		return;
	}
}

void NTSoundDevice::Update()
{
	if (nullptr != SoundSystem)
	{
		SoundSystem->update();
	}
}

void NTSoundDevice::Destroy()
{
	if (nullptr != NTSoundDevice::SoundSystem)
	{
		NTSoundDevice::SoundSystem->release();
		NTSoundDevice::SoundSystem = nullptr;
	}
}