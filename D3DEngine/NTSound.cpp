#include "PreCom.h"
#include "NTSound.h"

#include <atlstr.h>

NTSound::NTSound() : Sound(nullptr)
{
}


NTSound::~NTSound()
{
	if (nullptr != NTSoundDevice::SoundSystem)
	{
		if (nullptr != Sound)
		{
			Sound->release();
		}
	}
}

bool NTSound::Load()
{
	std::string Str = CW2A(GetPath(), CP_UTF8);

	if (FMOD_OK != NTSoundDevice::SoundSystem->createSound(Str.c_str(), FMOD_DEFAULT, nullptr, &Sound))
	{
		tassert(true);
		return false;
	}

	tassert(nullptr == Sound);

	if (nullptr == Sound)
	{
		return false;
	}

	return true;
}
