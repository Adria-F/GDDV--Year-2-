#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_gamecontroller.h"
#include "SDL\include\SDL_mouse.h"

#define MAX_KEYS 300
#define MAX_BUTTONS 16
#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum MOUSE_BUTTON_STATE
{
	MOUSE_BUTTON_IDLE = 0,
	MOUSE_BUTTON_DOWN,
	MOUSE_BUTTON_REPEAT,
	MOUSE_BUTTON_UP
};

enum KEY_CHAR
{
	W,
	A,
	S,
	D
};

enum PAD_BUTON_STATE
{
	BUTTON_IDLE = 0,
	BUTTON_DOWN,
	BUTTON_REPEAT,
	BUTTON_UP
};

enum CORNER_STATE
{
	CORNER_IDLE = 0,
	CORNER_1,
	CORNER_2,
	CORNER_3,
	CORNER_4
};

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

public:

	KEY_STATE keyboard[MAX_KEYS];
	PAD_BUTON_STATE controller[MAX_BUTTONS];
	MOUSE_BUTTON_STATE mouse[MAX_MOUSE_BUTTONS];
	SDL_Cursor* cursor;
	SDL_GameController *game_controller = nullptr;
	KEY_CHAR last_key;
	iPoint mouse_pos;
	CORNER_STATE corner_pressed;
	iPoint initial_mouse_pos;
	Collider* mouse_collider;
	Collider* moving_collider;
};

#endif // __ModuleInput_H__