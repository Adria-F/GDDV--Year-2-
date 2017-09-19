#include "Globals.h"
#include "ModuleWelcomeTitle.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"
#include "ModulePlayer.h" //temp

ModuleWelcomeTitle::ModuleWelcomeTitle(char* spritesheet)
{
	// Background

	spritesheet_path = spritesheet;
}

ModuleWelcomeTitle::~ModuleWelcomeTitle()
{}

// Load assets
bool ModuleWelcomeTitle::Start()
{
	LOG("Loading WelcomeTitle scene");
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;
	bool ret = true;
	App->render->moving_scene = false;
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	graphics = App->textures->Load("assets/maps/cellmap.png");
	//Walls colliders
	App->collision->AddCollider({ 120, 120, 40, 192 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 160, 168, 100, 48 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 160, 264, 160, 48 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 320, 264, 140, 24 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 360, 192, 20, 72 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 340, 216, 20, 24 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 400, 168, 20, 96 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 320, 144, 60, 24 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 320, 72, 160, 24 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 440, 96, 20, 120 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 280, 72, 40, 240 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 80, 384, 240, 48 }, COLLIDER_WALL, this);
	App->collision->AddCollider({ 460, 360, 100, 96 }, COLLIDER_WALL, this);

	LOG("Loading music");
	App->audio->Load("assets/music/welcome_title.ogg", App->audio->MUSIC);
	//App->audio->Play(App->audio->MUSIC);

	fading = false;

	App->ui->Enable();
	App->collision->Enable();
	//App->player1->Enable();

	return ret;
}

// UnLoad assets
bool ModuleWelcomeTitle::CleanUp()
{
	LOG("Unloading WelcomeTitle scene");
	if (graphics != nullptr)
	{
		App->textures->Unload(graphics);
		graphics = nullptr;
	}

	App->audio->Stop();
	App->ui->Disable();

	return true;
}

// Update: draw background

update_status ModuleWelcomeTitle::Update()
{
	// Draw everything --------------------------------------


	App->render->DrawQuad({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 24, 8, 8, 255);
	//App->render->Blit(graphics, App->render->camera.x, App->render->camera.y, nullptr, 0.75f);


	return UPDATE_CONTINUE;
}