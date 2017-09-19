#include "Application.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include <stdio.h>

ModuleUI::ModuleUI()
{


	char_pos.w = 15;
	char_pos.h = 13;
	char_pos.y = 12;

	notification = { 0, 0, 73, 15 };
	marion_rect = { 8, 12, 14, 13 };
	ash_rect = { 28, 12, 11, 13 };

	photo_rec = {8, 8, 96, 96};
	text_rec = {121, 127, 54, 21};
	box_rec = {6, 4, 32, 52};

	bomb_rect = { 2, 1, 12, 15 };

	marion_anim.PushBack({ 346, 142, 128, 108 });
	marion_anim.PushBack({ 177, 140, 128, 108 });
	marion_anim.PushBack({ 17, 141, 128, 108 });
	marion_anim.PushBack({ 177, 140, 128, 108 });
	marion_anim.loop = true;
	marion_anim.speed = 0.3f;
	
	ash_anim.PushBack({ 13, 11, 128, 108 });
	ash_anim.PushBack({ 141, 11, 128, 108 });
	ash_anim.PushBack({ 270, 11, 128, 108 });
	ash_anim.PushBack({ 394, 11, 128, 108 });
	ash_anim.loop = true;
	ash_anim.speed = 0.4f;

}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	game_over_bckg.w = SCREEN_WIDTH;
	game_over_bckg.h = SCREEN_HEIGHT;

	font_score = App->fonts->Load("assets/UI/ui_fonts.png", "udP:0123456789 ", 3);
	
	debugging_text = App->textures->Load("assets/UI/debugging_on.png");
	godMode_text = App->textures->Load("assets/UI/godmode_on.png");


	anim_pos = 127.0f;
	anim_speed = 0.1f;

	App->fonts->Enable();

	return true;
}

bool ModuleUI::CleanUp()
{
	Deactivate();
	
	App->textures->Unload(debugging_text);
	App->textures->Unload(godMode_text);
	
	App->fonts->Unload(font_score);
	App->fonts->Disable();

	return true;
}

update_status ModuleUI::Update()
{
	/*if (App->render->debugging)
	{
		App->render->Blit(debugging_text, 151, 290, &notification); // 0, 305
	}
	if (App->render->god_mode)
	{
		App->render->Blit(godMode_text, 151, 305, &notification);
	}*/

	return UPDATE_CONTINUE;
}