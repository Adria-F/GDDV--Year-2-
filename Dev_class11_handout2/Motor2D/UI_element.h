#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"

struct SDL_Texture;

enum element_type
{
	TEXT,
	IMAGE,
	BUTTON,
	//SLIDER,
	INPUTTEXT
};

enum element_state
{
	STANDBY,
	MOUSEOVER,
	CLICKED
};

class UI_element
{
public:

	UI_element()
	{}

	UI_element(int x, int y, element_type type, SDL_Rect section, j1Module* callback, SDL_Texture* texture = nullptr): position({x, y}), element_type(type), section(section), callback(callback), texture(texture)
	{}

	virtual ~UI_element()
	{
		if (texture != nullptr && texture != App->gui->GetAtlas())
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

	virtual void setOutlined(bool isOutlined)
	{}

	virtual void BlitElement()
	{}

public:

	SDL_Texture* texture = nullptr;
	iPoint position;
	SDL_Rect section;
	element_type element_type;
	element_state state = STANDBY;
	j1Module* callback = nullptr;
	bool hovering = false;
};

#endif // !__UI_ELEMENT__
