#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include <iostream>

ModuleInput::ModuleInput() : Module()
{
	for (uint i = 0; i < MAX_KEYS; ++i)
		keyboard[i] = KEY_IDLE;

	for (uint i = 0; i < MAX_BUTTONS; ++i)
		controller[i] = BUTTON_IDLE;

	for (uint i = 0; i < MAX_MOUSE_BUTTONS; i++)
		mouse[i] = MOUSE_BUTTON_IDLE;

	SDL_Init(SDL_INIT_GAMECONTROLLER);
	if (SDL_NumJoysticks() < 1)
	{
		LOG("Warning: No joysticks connected!\n");
	}
	else
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i))
			{
				game_controller = SDL_GameControllerOpen(i);
				std::cout << SDL_GameControllerMapping(game_controller) << std::endl;
				break;
			}
			else
			{
				LOG("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}

	}
	cursor = SDL_GetCursor();
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	SDL_Init(SDL_INIT_GAMECONTROLLER); //?
	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}	

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	//mouse_collider->SetPos(mouse_pos.x, mouse_pos.y);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint8 button[MAX_BUTTONS];
	button[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	button[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	button[SDL_CONTROLLER_BUTTON_A] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_A);
	button[SDL_CONTROLLER_BUTTON_B] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_B);
	button[SDL_CONTROLLER_BUTTON_Y] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_Y);
	button[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	button[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = SDL_GameControllerGetButton(App->input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		if (button[i] == 1)
		{
			if (controller[i] == BUTTON_IDLE)
				controller[i] = BUTTON_DOWN;
			else
				controller[i] = BUTTON_REPEAT;
		}
		else
			if (controller[i] == BUTTON_REPEAT || controller[i] == BUTTON_DOWN)
				controller[i] = BUTTON_UP;
			else
				controller[i] = BUTTON_IDLE;
	}

	Uint8 mouse_button[MAX_MOUSE_BUTTONS];
	mouse_button[SDL_BUTTON_LEFT] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	mouse_button[SDL_BUTTON_RIGHT] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);


	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		if (mouse_button[i] == 1)
		{
			if (mouse[i] == MOUSE_BUTTON_IDLE)
				mouse[i] = MOUSE_BUTTON_DOWN;
			else
				mouse[i] = MOUSE_BUTTON_REPEAT;
		}
		else
		{
			if (mouse[i] == MOUSE_BUTTON_REPEAT || mouse[i] == MOUSE_BUTTON_DOWN)
			{
				mouse[i] = MOUSE_BUTTON_UP;
				corner_pressed = CORNER_IDLE;
				moving_collider = nullptr;
			}
			else
				mouse[i] = MOUSE_BUTTON_IDLE;
		}
	}

	if(keyboard[SDL_SCANCODE_ESCAPE])

		return update_status::UPDATE_STOP;

	
	if (keyboard[SDL_SCANCODE_F1] == KEY_STATE::KEY_DOWN || controller[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] == PAD_BUTON_STATE::BUTTON_DOWN)
		{
			if (App->render->debugging)
				App->render->debugging = false;
			else
				App->render->debugging = true;
		}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_MOUSE && c2->type == COLLIDER_CORNER && mouse[SDL_BUTTON_LEFT] == MOUSE_BUTTON_DOWN)
	{
		LOG("You clicked a corner!");
		if (c2->parent->corner1 == c2)
			corner_pressed = CORNER_1;
		else if (c2->parent->corner2 == c2)
			corner_pressed = CORNER_2;
		else if (c2->parent->corner3 == c2)
			corner_pressed = CORNER_3;
		else if (c2->parent->corner4 == c2)
			corner_pressed = CORNER_4;
		initial_mouse_pos.x = mouse_pos.x;
		initial_mouse_pos.y = mouse_pos.y;
		moving_collider = c2->parent;
	}
	else if (c1->type == COLLIDER_MOUSE && mouse[SDL_BUTTON_LEFT] == MOUSE_BUTTON_DOWN)
	{
		LOG("You clicked into a collider!");
		moving_collider = c2;
		initial_mouse_pos.x = mouse_pos.x;
		initial_mouse_pos.y = mouse_pos.y;
	}	
}