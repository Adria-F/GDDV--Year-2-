#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1UIManager.h"

j1UIManager::j1UIManager() : j1Module()
{
	name.create("gui");
}

// Destructor
j1UIManager::~j1UIManager()
{}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UIManager::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1UIManager::PreUpdate()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		for (p2List_item<Button*>* item = buttons.start; item; item = item->next)
		{
			if (x > item->data->position.x && x < item->data->position.x + item->data->standby.w && y > item->data->position.y && y < item->data->position.y + item->data->standby.h)
			{
				item->data->clicked = true;
				if (item->data->type == CHECKBOX)
				{
					item->data->tick = !item->data->tick;
				}
				break;
			}
		}
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		for (p2List_item<Button*>* item = buttons.start; item; item = item->next)
		{
			if (item->data->clicked)
			{
				item->data->clicked = false;
				break;
			}
		}
	}

	return true;
}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	blitImages();
	blitTexts();
	blitButtons();
	blitSliders();

	return true;
}

// Called before quitting
bool j1UIManager::CleanUp()
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

	p2List_item<Button*>* Bitem;
	Bitem = buttons.start;
	while (Bitem != NULL)
	{
		RELEASE(Bitem->data);
		Bitem = Bitem->next;
	}
	buttons.clear();

	p2List_item<Slider*>* Sitem;
	Sitem = sliders.start;
	while (Sitem != NULL)
	{
		RELEASE(Sitem->data);
		Sitem = Sitem->next;
	}
	sliders.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}

Text* j1UIManager::createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color)
{
	Text* ret = new Text(text, x, y, font, color);
	texts.add(ret);
	
	return ret;
}

Image* j1UIManager::createImage(int x, int y, SDL_Texture* texture)
{
	Image* ret = new Image(texture, x, y, NULL);
	images.add(ret);
	
	return ret;
}

Image* j1UIManager::createImageFromAtlas(int x, int y, SDL_Rect section)
{
	SDL_Rect* rect = new SDL_Rect(section);
	Image* ret = new Image(atlas, x, y, rect);
	images.add(ret);

	return ret;
}

Button * j1UIManager::createCheckBox(int x, int y, SDL_Texture * texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnClick, Tick, CHECKBOX);
	buttons.add(ret);

	return ret;
}

Slider * j1UIManager::createSlider(int x, int y, SDL_Texture * texture, SDL_Rect bar, SDL_Rect container, SDL_Rect button)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Slider* ret = new Slider(x, y, usingTexture, bar, container, button);
	sliders.add(ret);

	return ret;
}

Button* j1UIManager::createButton(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Button* ret = new Button(text, font, color, x, y, usingTexture, standby, OnMouse, OnClick, LINK);
	buttons.add(ret);
	
	return ret;
}

void j1UIManager::blitTexts()
{
	for (p2List_item<Text*>* item = texts.start; item; item = item->next)
	{
		item->data->createTexture();
		if (item->data->outline)
			App->render->Blit(item->data->outline, item->data->position.x + item->data->outline_offset.x, item->data->position.y + item->data->outline_offset.y, NULL, false);
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, NULL, false);
	}
}

void j1UIManager::blitImages()
{
	for (p2List_item<Image*>* item = images.start; item; item = item->next)
	{
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, item->data->section, false);
	}
}

void j1UIManager::blitButtons()
{
	for (p2List_item<Button*>* item = buttons.start; item; item = item->next)
	{
		if (item->data->clicked)
			App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->OnClick, false);
		else
			App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->standby, false);
		if (item->data->type == CHECKBOX && item->data->tick)
		{
			App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->Tick, false);
		}
		else if (item->data->type == LINK)
		{
			item->data->text->createTexture();
			item->data->text->position.x = item->data->position.x + item->data->standby.w / 2 - item->data->text->tex_width / 2;
			item->data->text->position.y = item->data->position.y + item->data->standby.h / 2 - item->data->text->tex_height / 2;
			if (item->data->text->outline)
				App->render->Blit(item->data->text->outline, item->data->text->position.x + item->data->text->outline_offset.x, item->data->text->position.y + item->data->text->outline_offset.y, NULL, false);
			App->render->Blit(item->data->text->texture, item->data->text->position.x, item->data->text->position.y, NULL, false);
		}
	}
}

void j1UIManager::blitSliders()
{
	for (p2List_item<Slider*>* item = sliders.start; item; item = item->next)
	{
		item->data->bar.w = ((item->data->bar.w - 14) * item->data->progress) / 100;
		if (item->data->bar.w < 0)
			item->data->bar.w = 0;

		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->container);
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->bar);
		App->render->Blit(item->data->texture, item->data->position.x + item->data->bar.w, item->data->position.y - 2, &item->data->button);
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
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}
}

void Text::createTexture()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}
	
	uint outline_width, outline_height;
	App->font->setFontOutline(font, 2);
	outline = App->font->Print(text, {0, 0, 0, 255}, font); //Outlined texture
	App->tex->GetSize(outline, outline_width, outline_height);

	App->font->setFontOutline(font, 0);
	texture = App->font->Print(text, color, font); //Normal texture
	App->tex->GetSize(texture, tex_width, tex_height);

	outline_offset.x = tex_width - outline_width;
	outline_offset.x /= 2;
	outline_offset.y = outline_offset.x;
	
}

Image::~Image()
{
	if (texture != nullptr && texture != App->UIManager->GetAtlas())
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
	if (texture != nullptr && texture != App->UIManager->GetAtlas())
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

Slider::~Slider()
{
	if (texture != nullptr && texture != App->UIManager->GetAtlas())
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}

void Slider::setProgress(int newProgress)
{
	progress = newProgress;

	if (progress < 0)
		progress = 0;
	else if (progress > 100)
		progress = 100;
}
