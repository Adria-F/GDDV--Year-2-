#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		PhysBody* body = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25);
		for (int i = 0; i < MAX_CIRCLES; i++)
		{
			if (App->physics->circles[i] == nullptr)
			{
				App->physics->circles[i] = body;
				break;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		PhysBody* body = App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 50, 25);
		for (int i = 0; i < MAX_RECTANGLES; i++)
		{
			if (App->physics->rectangles[i] == nullptr)
			{
				App->physics->rectangles[i] = body;
				break;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		PhysBody* body = App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), 1);
		for (int i = 0; i < MAX_CHAINS; i++)
		{
			if (App->physics->chains[i] == nullptr)
			{
				App->physics->chains[i] = body;
				break;
			}
		}
	}
	
	// TODO 7: Draw all the circles using "circle" texture
	int x, y;
	for (int i = 0; i < MAX_CIRCLES; i++)
	{
		if (App->physics->circles[i] != nullptr)
		{
			App->physics->circles[i]->getPosition(&x, &y);
			App->renderer->Blit(circle, x, y);
		}
	}
	for (int i = 0; i < MAX_RECTANGLES; i++)
	{
		if (App->physics->rectangles[i] != nullptr)
		{
			App->physics->rectangles[i]->getPosition(&x, &y);
			App->renderer->Blit(box, x, y);
		}
	}
	for (int i = 0; i < MAX_CHAINS; i++)
	{
		if (App->physics->chains[i] != nullptr)
		{
			App->physics->chains[i]->getPosition(&x, &y);
			App->renderer->Blit(rick, x, y);
		}
	}

	return UPDATE_CONTINUE;
}
