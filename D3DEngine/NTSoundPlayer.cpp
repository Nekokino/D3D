#include "PreCom.h"
#include "NTSoundPlayer.h"
#include "ResourceSystem.h"



NTSoundPlayer::NTSoundPlayer()
{
}


NTSoundPlayer::~NTSoundPlayer()
{
}

FMOD_RESULT NTSoundPlayer::SoundCallBack(FMOD_CHANNELCONTROL * _ChannelControl, FMOD_CHANNELCONTROL_TYPE _ControlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _CallbackType, void * _CommandData1, void * _CommandData2)
{
	if (_ControlType == FMOD_CHANNELCONTROL_CHANNEL)
	{
		FMOD::Channel* CallPtr = (FMOD::Channel*)_ChannelControl;

		NTSoundPlayer* Ptr = nullptr;
		CallPtr->getUserData((void**)(&Ptr));

		switch (_CallbackType)
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
		{
			Ptr->End();
			break;
		}
		default:
			break;
		}
	}

	return FMOD_OK;
}

void NTSoundPlayer::End()
{
	bPlay = false;
	Channel = nullptr;
}

bool NTSoundPlayer::SoundPlay(const wchar_t * _SoundName)
{
	Sound = ResourceSystem<NTSound>::Find(_SoundName);

	tassert(nullptr == Sound);
	
	FMOD_RESULT Result = NTSoundDevice::SoundSystem->playSound(Sound->Sound, nullptr, false, &Channel);

	Channel->setCallback(SoundCallBack);
	Channel->setUserData(this);

	bPlay = true;

	if (FMOD_OK != Result)
	{
		return false;
	}

	return true;
}

void NTSoundPlayer::StopSound()
{
	Channel->stop();
	bPlay = false;
}
