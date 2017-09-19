#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio()
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	return true;
}

void ModuleAudio::Play(sound_type type)
{
	switch (type)
	{
		case sound_type::MUSIC:
		{
			Mix_PlayMusic(music, -1);
			break;
		}
		case sound_type::EFFECT:
		{
			Mix_PlayChannel(-1, effect, 0);
			break;
		}
	}
}

void ModuleAudio::Stop()
{
	Mix_HaltChannel(-1);
	if (music != nullptr)
	{
		Mix_FreeMusic(music);
		music = nullptr;
	}
	if (effect != nullptr)
	{
		Mix_FreeChunk(effect);
		effect = nullptr;
	}
}

bool ModuleAudio::Load(const char* path, sound_type type)
{
	switch (type)
	{
	case sound_type::MUSIC:
	{
		music = Mix_LoadMUS(path);
		break;
	}
	case sound_type::EFFECT:
	{
		effect = Mix_LoadWAV(path);
		break;
	}
	}

	return true;
}

bool ModuleAudio::CleanUp()
{
	return true;
}