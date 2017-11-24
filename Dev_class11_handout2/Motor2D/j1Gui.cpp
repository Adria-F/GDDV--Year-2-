#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	blitImages();
	blitTexts();
	blitButtons();

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<Text*>* Titem;
	Titem = texts.start;
	while (Titem != NULL)
	{
		RELEASE(Titem->data);
		Titem = Titem->next;
	}
	texts.clear();

	p2List_item<Image*>* Iitem;
	Iitem = images.start;
	while (Iitem != NULL)
	{
		RELEASE(Iitem->data);
		Iitem = Iitem->next;
	}
	images.clear();

	p2List_item<Image*>* Bitem;
	Bitem = images.start;
	while (Bitem != NULL)
	{
		RELEASE(Bitem->data);
		Bitem = Bitem->next;
	}
	buttons.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

Text* j1Gui::createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color)
{
	Text* ret = new Text(text, x, y, font, color);
	texts.add(ret);
	
	return ret;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture)
{
	Image* ret = new Image(texture, x, y, NULL);
	images.add(ret);
	
	return ret;
}

Image* j1Gui::createImageFromAtlas(int x, int y, SDL_Rect section)
{
	SDL_Rect* rect = new SDL_Rect(section);
	Image* ret = new Image(atlas, x, y, rect);
	images.add(ret);

	return ret;
}

Button* j1Gui::createButton(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, button_type type)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Button* ret = new Button(text, font, color, x, y, usingTexture, standby, OnMouse, OnClick, type);
	buttons.add(ret);
	
	return ret;
}

Button* j1Gui::createButton(Button* button)
{
	Button* ret = new Button(*button);
	buttons.add(ret);
	
	return ret;
}

void j1Gui::blitTexts()
{
	for (p2List_item<Text*>* item = texts.start; item; item = item->next)
	{
		item->data->createTexture();
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, NULL, false);
	}
}

void j1Gui::blitImages()
{
	for (p2List_item<Image*>* item = images.start; item; item = item->next)
	{
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, item->data->section, false);
	}
}

void j1Gui::blitButtons()
{
	for (p2List_item<Button*>* item = buttons.start; item; item = item->next)
	{
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->standby, false);
		item->data->text->createTexture();
		item->data->text->position.x = item->data->position.x + item->data->standby.w/2 - item->data->text->tex_width/2;
		item->data->text->position.y = item->data->position.y + item->data->standby.h / 2 - item->data->text->tex_height / 2;
		App->render->Blit(item->data->text->texture, item->data->text->position.x, item->data->text->position.y, NULL, false);
	}
}

// class Gui ---------------------------------------------------

Text::~Text()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}

void Text::createTexture()
{
	texture = App->font->Print(text, color, font);
	App->tex->GetSize(texture, tex_width, tex_height);
}

Image::~Image()
{
	if (texture != nullptr && texture != App->gui->GetAtlas())
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
	if (section != nullptr)
	{
		delete section;
		section = nullptr;
	}
}

Button::~Button()
{
	if (texture != nullptr && texture != App->gui->GetAtlas())
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

bool Button::clicked()
{
	return false;
}
