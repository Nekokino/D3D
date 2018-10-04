#pragma once
#include "NTComponent.h"
#include "NTSound.h"

class NTSoundPlayer : public NTComponent
{
private:
	static FMOD_RESULT __stdcall SoundCallBack(FMOD_CHANNELCONTROL* _ChannelControl, FMOD_CHANNELCONTROL_TYPE _ControlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _CallbackType,
		void* _CommandData1, void* _CommandData2);

private:
	Autoptr<NTSound> Sound;
	FMOD::Channel* Channel;
	bool bPlay;

private:
	void End();

public:
	bool SoundPlay(const wchar_t* _SoundName);
	void StopSound();

public:
	bool IsPlay()
	{
		return bPlay;
	}
public:
	NTSoundPlayer();
	~NTSoundPlayer();
};

