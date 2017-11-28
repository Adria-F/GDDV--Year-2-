#ifndef __UI_BUTTON__

#include "UI_element.h"
#include "UI_Text.h"

enum button_type
{
	LINK,
	CHECKBOX
};

class Button : public UI_element
{
public:

	Button()
	{}

	Button(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, button_type type, j1Module* callback) : UI_element(x, y, element_type::BUTTON, standby, callback, texture),
		OnMouse(OnMouse),
		Tick(OnMouse),
		OnClick(OnClick),
		type(type)
	{
		this->text = new Text(text, position.x, position.y, font, color, nullptr);
	}

	Button(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, button_type type, j1Module* callback) : UI_element(x, y, element_type::BUTTON, standby, callback, texture),
		Tick(Tick),
		OnClick(OnClick),
		type(type)
	{}

	~Button();

	void setOutlined(bool isOutlined);
	void BlitElement();

public:

	Text* text = nullptr;
	SDL_Rect OnMouse;
	SDL_Rect Tick;
	SDL_Rect OnClick;
	button_type type = LINK;
	bool tick = false;
	bool clicked = false;
};

#endif // !__UI_BUTTON__
