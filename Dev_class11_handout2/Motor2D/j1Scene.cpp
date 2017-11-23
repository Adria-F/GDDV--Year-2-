#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Fonts.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{	
	App->gui->createImage(0, 0, App->tex->Load("textures/login_background.png")); //Background Image
	App->gui->createImageFromAtlas(10, 10, { 230, 19, 179, 80 }); //Wow Logo
	App->gui->createImageFromAtlas(430, 470, { 39, 32, 66, 43 }); //Blizzard Logo
	App->gui->createImageFromAtlas(25, 450, { 9, 109, 128, 40 }); //Notice Image

	//Buttons ---temp---
	App->gui->createImageFromAtlas(400, 375, { 13, 188, 129, 25 }); //Login
	App->gui->createImageFromAtlas(25, 350, { 13, 188, 129, 25 }); //Manage Account
	App->gui->createImageFromAtlas(25, 385, { 13, 188, 129, 25 }); //Community Site
	App->gui->createImageFromAtlas(18, 425, { 19, 266, 18, 17 }); //Checkbox
	App->gui->createImageFromAtlas(810, 348, { 13, 188, 129, 25 }); //Terms of Use
	App->gui->createImageFromAtlas(810, 315, { 13, 188, 129, 25 }); //Credits
	App->gui->createImageFromAtlas(810, 280, { 13, 188, 129, 25 }); //Cinematics
	App->gui->createImageFromAtlas(810, 475, { 13, 188, 129, 25 }); //Quit

	//Input boxes
	App->gui->createImageFromAtlas(402, 320, { 311, 228, 128, 20 }); //Account Password
	App->gui->createImageFromAtlas(402, 260, { 311, 228, 128, 20 }); //Account Name

	//Texts
	SDL_Color text_color = { 255, 255, 0, 255 };
	_TTF_Font* text_font = App->font->Load("fonts/wow/ARIALN.ttf");
	App->gui->createText("Remember Account Name", 45, 425, text_font, text_color);
	App->gui->createText("Version 2.0.12 (6546) (Release)", 10, 498, text_font, text_color);
	App->gui->createText("Mar 30 2007", 10, 516, text_font, text_color);
	App->gui->createText("Copyright 2004-2007  Blizzard Entertainment. All Rights Reserved.", 275, 516, text_font, text_color);
	App->gui->createText("Account Password", 412, 300, text_font, text_color);
	App->gui->createText("Account Name", 425, 240, text_font, text_color);
	App->gui->createText("WoWops.org TBC", 840, 385, text_font, {100, 100, 100, 255});

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
