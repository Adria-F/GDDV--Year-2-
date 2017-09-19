#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

class ModuleAudio : public Module
{

public:
	enum sound_type
	{
		MUSIC,
		EFFECT
	};

	ModuleAudio();
	virtual ~ModuleAudio();

	bool Load(const char* path, sound_type type);
	void Play(sound_type type);
	void Stop();

	bool Init();
	bool CleanUp();

private:
	Mix_Music* music = nullptr;
	Mix_Chunk* effect = nullptr;
};
#endif