#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_InputBox.h"
#include "UI_Window.h"

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
	bool ret = true;

	int x, y;
	App->input->GetMousePosition(x, y);
	int scale = App->win->GetScale();
	UI_element* element = nullptr;

	//Get element to interact with
	if (draggingElement != nullptr)
		element = draggingElement;
	else
	{
		for (p2List_item<UI_element*>* item = UI_elements.end; item; item = item->prev)
		{
			iPoint globalPos = item->data->calculateAbsolutePosition();
			if (x > globalPos.x && x < globalPos.x + item->data->section.w / scale && y > globalPos.y && y < globalPos.y + item->data->section.h / scale && element == nullptr && item->data->dragable)
			{
				element = item->data;
			}
			else if (item->data->hovering)
			{
				item->data->hovering = false;
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_LEAVE);
			}
		}
	}

	//Send events related to UI elements
	if (element != nullptr)
	{
		if (!element->hovering)
		{
			element->hovering = true;
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_ENTER);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (element->callback != nullptr)
			{
				ret = element->callback->OnUIEvent(element, MOUSE_LEFT_CLICK);
			}
			if (element->dragable)
			{
				element->Start_Drag();
				draggingElement = element;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			if (element->callback != nullptr)
			{
				element->callback->OnUIEvent(element, MOUSE_LEFT_RELEASE);
			}
			if (element->dragable)
			{
				element->End_Drag();
				draggingElement = nullptr;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			if (element->callback != nullptr)
				ret = element->callback->OnUIEvent(element, MOUSE_RIGHT_CLICK);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_RIGHT_RELEASE);
		}
	}
	if (focus != nullptr)
	{
		element = focus->data;
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			focus->data->callback->OnUIEvent(element, MOUSE_LEFT_CLICK);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
			focus->data->callback->OnUIEvent(element, MOUSE_LEFT_RELEASE);
		if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		{
			focus = focus->next;
			if (focus == nullptr)
				focus = UI_elements.start;
			while (focus->data->callback == nullptr)
			{
				focus = focus->next;
				if (focus == nullptr)
					focus = UI_elements.start;
			}
			if (focus->data != element)
			{
				element->callback->OnUIEvent(element, FOCUS_LOST);
				focus->data->callback->OnUIEvent(focus->data, FOCUS_RECEIVED);
			}
		}
		iPoint globalPos = focus->data->calculateAbsolutePosition();
		if ((x < globalPos.x || x > globalPos.x + focus->data->section.w / scale || y < globalPos.y || y > globalPos.y + focus->data->section.h / scale))
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				focus->data->callback->OnUIEvent(focus->data, FOCUS_LOST);
				focus = nullptr;
			}
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		focus = UI_elements.start;
		while (focus->data->callback == nullptr)
		{
			focus = focus->next;
			if (focus == nullptr)
				focus = UI_elements.start;
		}
		focus->data->callback->OnUIEvent(focus->data, FOCUS_RECEIVED);
	}
	
	return ret;
}

bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{	
	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next)
	{
		if (item->data->moving)
			item->data->Mouse_Drag();

		if (item->data->parent == nullptr)
			item->data->BlitElement();
	}

	if (UI_Debug)
		DebugDraw();

	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next) //Input Text reading
	{
		InputBox* inputBox = nullptr;
		if (item->data->element_type == INPUTTEXT)
			inputBox = (InputBox*)item->data;
		else
			continue;
		if (inputBox->reading)
		{
			SDL_StartTextInput();
			inputBox->readInput();
			/*if (!App->input->pauseInput)
				App->input->pauseInput = true;*/
			break;
		}
		else
		{
			SDL_StopTextInput();
			/*if (App->input->pauseInput)
				App->input->pauseInput = false;*/
		}

	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_element*>* item;
	item = UI_elements.start;
	while (item != NULL)
	{
		if (item == focus)
			focus = nullptr;
		RELEASE(item->data);
		item = item->next;
	}
	UI_elements.clear();

	return true;
}

void j1Gui::DebugDraw()
{
	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next)
	{
		SDL_Rect box;
		box.x = item->data->calculateAbsolutePosition().x;
		box.y = item->data->calculateAbsolutePosition().y;
		box.w = item->data->section.w;
		box.h = item->data->section.h;
		if (item == focus)
			App->render->DrawQuad(box, 255, 255, 0, 255, false);
		else
			App->render->DrawQuad(box, 255, 0, 0, 255, false);
	}

}

void j1Gui::setFocus(UI_element * element)
{
	if (element == nullptr && focus != nullptr)
	{
		focus->data->callback->OnUIEvent(focus->data, FOCUS_LOST);
		focus = nullptr;
	}
	else if (focus == nullptr || focus->data != element)
	{
		if (focus != nullptr && focus->data->callback != nullptr)
		{
			focus->data->callback->OnUIEvent(focus->data, FOCUS_LOST);
		}
		focus = UI_elements.At(UI_elements.find(element));
		if (focus->data->callback != nullptr)
		{
			focus->data->callback->OnUIEvent(element, FOCUS_RECEIVED);
		}
	}
	else
		LOG("This element already has the focus!");
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

Text* j1Gui::createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback)
{
	Text* ret = new Text(text, x, y, font, color, callback);
	UI_elements.add(ret);
	
	return ret;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture, j1Module* callback)
{
	Image* ret = new Image(texture, x, y, {0, 0, 0, 0}, callback);
	UI_elements.add(ret);
	
	return ret;
}

Image* j1Gui::createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback)
{
	Image* ret = new Image(atlas, x, y, section, callback);
	UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createCheckBox(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnClick, Tick, CHECKBOX, callback);
	UI_elements.add(ret);

	return ret;
}

InputBox* j1Gui::createInputBox(char* defaultText, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	InputBox* ret = new InputBox(defaultText, font, color, x, y, usingTexture, section, callback);
	UI_elements.add(ret);

	return ret;
}

Window* j1Gui::createWindow(int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module * callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Window* ret = new Window(usingTexture, x, y, section, callback);
	UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Button* ret = new Button(x, y, usingTexture, standby, OnMouse, OnClick, LINK, callback);
	UI_elements.add(ret);
	
	return ret;
}

// class Gui ---------------------------------------------------
