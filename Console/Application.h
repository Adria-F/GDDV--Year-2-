#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 14

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleSceneMine;
class Player;
class ModuleFadeToBlack;
class Module;
class ModuleAudio;
class ModuleWelcomeTitle;
class ModuleDebugMode;
class ModuleParticles;
class ModuleCollision;
class ModuleFonts;
class ModuleUI;
class Chat;

class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	Player* player1;
	Player* player2;
	ModuleFadeToBlack* fade;
	ModuleAudio* audio;
	ModuleWelcomeTitle* welcometitle;
	ModuleParticles* particles;
	ModuleCollision* collision;
	ModuleFonts* fonts;
	Chat* chat;
	ModuleUI* ui;
	
	int screen_height = 480;
	int screen_width = 720;

public:

	Application(char* spritesheet);
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__