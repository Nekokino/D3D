#pragma once
#include "NTResource.h"
#include "NTSoundDevice.h"

class NTSound final : public NTResource
{
private:
	friend class NTSoundPlayer;

private:
	FMOD::Sound* Sound;

public:
	bool Load();
public:
	NTSound();
	~NTSound();
};

