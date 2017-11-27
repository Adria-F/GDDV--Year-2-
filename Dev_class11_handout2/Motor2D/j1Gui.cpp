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
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		for (p2List_item<Button*>* item = buttons.start; item; item = item->next) //Buttons
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
		for (p2List_item<inputText*>* item = inputTexts.start; item; item = item->next) //Input Text
		{
			if (x > item->data->position.x && x < item->data->position.x + item->data->box.w && y > item->data->position.y && y < item->data->position.y + item->data->box.h)
			{
				item->data->reading = true;
				SDL_StartTextInput();
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
bool j1Gui::PostUpdate()
{	
	blitImages();
	blitTexts();
	blitButtons();
	blitInputTexts();

	for (p2List_item<inputText*>* item = inputTexts.start; item; item = item->next) //Input Text reading
	{
		if (item->data->reading)
		{
			item->data->readInput();
			break;
		}
	}

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

	p2List_item<Button*>* Bitem;
	Bitem = buttons.start;
	while (Bitem != NULL)
	{
		RELEASE(Bitem->data);
		Bitem = Bitem->next;
	}
	buttons.clear();

	p2List_item<inputText*>* Initem;
	Initem = inputTexts.start;
	while (Initem != NULL)
	{
		RELEASE(Initem->data);
		Initem = Initem->next;
	}
	inputTexts.clear();

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

Button * j1Gui::createCheckBox(int x, int y, SDL_Texture * texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnClick, Tick, CHECKBOX);
	buttons.add(ret);

	return ret;
}

inputText * j1Gui::createInputText(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture * texture, SDL_Rect section)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	inputText* ret = new inputText(font, color, x, y, usingTexture, section);
	inputTexts.add(ret);

	return ret;
}

Button* j1Gui::createButton(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Button* ret = new Button(text, font, color, x, y, usingTexture, standby, OnMouse, OnClick, LINK);
	buttons.add(ret);
	
	return ret;
}

void j1Gui::blitTexts()
{
	for (p2List_item<Text*>* item = texts.start; item; item = item->next)
	{
		item->data->createTexture();
		if (item->data->outline)
			App->render->Blit(item->data->outline, item->data->position.x + item->data->outline_offset.x, item->data->position.y + item->data->outline_offset.y, NULL, false);
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

void j1Gui::blitInputTexts()
{
	for (p2List_item<inputText*>* item = inputTexts.start; item; item = item->next)
	{
		App->render->Blit(item->data->texture, item->data->position.x, item->data->position.y, &item->data->box, false);

		if (item->data->text != nullptr)
		{
			item->data->text->createTexture();
			item->data->text->position.x = item->data->position.x + item->data->box.w / 2 - item->data->text->tex_width / 2;
			item->data->text->position.y = item->data->position.y + item->data->box.h / 2 - item->data->text->tex_height / 2;
			if (item->data->text->outline)
				App->render->Blit(item->data->text->outline, item->data->text->position.x + item->data->text->outline_offset.x, item->data->text->position.y + item->data->text->outline_offset.y, NULL, false);
			App->render->Blit(item->data->text->texture, item->data->text->position.x, item->data->text->position.y, NULL, false);
		}
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
	outline = App->font->Print(text.GetString(), {0, 0, 0, 255}, font); //Outlined texture
	App->tex->GetSize(outline, outline_width, outline_height);

	App->font->setFontOutline(font, 0);
	texture = App->font->Print(text.GetString(), color, font); //Normal texture
	App->tex->GetSize(texture, tex_width, tex_height);

	outline_offset.x = tex_width - outline_width;
	outline_offset.x /= 2;
	outline_offset.y = outline_offset.x;
	
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

inputText::~inputText()
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

void inputText::readInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && text->text.Length() > 0)
			{
				text->text.Cut(0, text->text.Length() - 1);
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			if (!(x > position.x && x < position.x + box.w && y > position.y && y <position.y + box.h)) //This event should be able to be detected in other modules
			{
				reading = false;
				SDL_StopTextInput();
			}
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			text->text += e.text.text;
		}
	}
}
