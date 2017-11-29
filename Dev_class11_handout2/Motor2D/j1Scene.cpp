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
#include "UI_element.h"
#include "UI_Text.h"

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
	SDL_Color text_color = { 255, 255, 255, 255 }; //255 215 70
	_TTF_Font* text_font = App->font->Load("fonts/wow/FRIZQUAD.ttf", 15);

	App->gui->createImage(0, 0, App->tex->Load("textures/login_background.png")); //Background Image
	App->gui->createButton("", text_font, text_color, 100, 100, NULL, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	text = App->gui->createText("Hello World", 120, 80, text_font, text_color, this);

	/*App->gui->createImageFromAtlas(10, 10, { 230, 19, 179, 80 }); //Wow Logo
	App->gui->createImageFromAtlas(430, 470, { 39, 32, 66, 43 }); //Blizzard Logo
	App->gui->createImageFromAtlas(25, 450, { 9, 109, 128, 40 }); //Notice Image

	//Buttons ---temp---
	App->gui->createButton("Login", text_font, text_color, 400, 375, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createButton("Manage Account", text_font, text_color, 25, 350, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createButton("Community Site", text_font, text_color, 25, 385, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createCheckBox(18, 425, NULL, { 19, 266, 18, 17 }, { 135, 266, 16, 15 }, { 240, 266, 17, 17 });
	App->gui->createButton("Terms of Use", text_font, text_color, 810, 348, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createButton("Credits", text_font, text_color, 810, 315, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createButton("Cinematics", text_font, text_color, 810, 280, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });
	App->gui->createButton("Quit", text_font, text_color, 810, 475, NULL, { 13, 188, 129, 25 }, { 13, 188, 129, 25 }, { 167, 188, 129, 25 });

	//Input boxes
	App->gui->createInputBox(text_font, {255, 255, 255, 255}, 402, 320, NULL, { 311, 228, 128, 20 }); //Account Password
	App->gui->createInputBox(text_font, { 255, 255, 255, 255 }, 402, 260, NULL, { 311, 228, 128, 20 }); //Account Name

	//Texts
	App->gui->createText("Remember Account Name", 45, 425, text_font, text_color);
	App->gui->createText("Version 2.0.12 (6546) (Release)", 10, 498, text_font, text_color);
	App->gui->createText("Mar 30 2007", 10, 516, text_font, text_color);
	App->gui->createText("Copyright 2004-2007  Blizzard Entertainment. All Rights Reserved.", 275, 516, text_font, text_color);
	App->gui->createText("Account Password", 412, 300, text_font, text_color);
	App->gui->createText("Account Name", 425, 240, text_font, text_color);
	App->gui->createText("WoWops.org TBC", 840, 385, text_font, {100, 100, 100, 255});*/

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
	bool ret = true;
	// Gui ---
	

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool j1Scene::OnUIEvent(UI_element* element, event_type event_type)
{
	if (event_type == MOUSE_ENTER || event_type == MOUSE_LEFT_RELEASE || event_type == MOUSE_RIGHT_RELEASE)
	{
		element->state = MOUSEOVER;
		if (element == (UI_element*)text)
		{
			text->setText("Hovering");
		}
	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;
		if (element == (UI_element*)text)
		{
			text->setText("Hello World");
		}
	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		element->state = CLICKED;
		if (element == (UI_element*)text)
		{
			text->setText("Left click");
		}
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
		if (element == (UI_element*)text)
		{
			text->setText("Right click");
		}
	}

	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
