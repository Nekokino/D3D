#pragma once

#ifdef _M_IX86
	#pragma comment(lib, "fmod_vc")
#else
	#pragma comment(lib, "fmod64_vc")
#endif

#include <fmod.hpp>

class NTSoundDevice
{
public:
	friend class WinCore;
	friend class NTSound;
	friend class NTSoundPlayer;

private:
	class StaticDestroyer
	{
		friend NTSoundDevice;
		~StaticDestroyer() { NTSoundDevice::Destroy(); }
	};
		
	friend StaticDestroyer;
	static StaticDestroyer Destroyer;

private:
	static FMOD::System* SoundSystem;
	static void Init();
	static void Update();
	static void Destroy();
public:
	NTSoundDevice();
	~NTSoundDevice();
};

