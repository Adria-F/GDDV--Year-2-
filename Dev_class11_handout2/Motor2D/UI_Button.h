#ifndef __UI_BUTTON__

#include "UI_element.h"

class Button : public UI_element
{
	Button()
	{}

	Button(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, button_type type, j1Module* callback) : UI_element(x, y, element_type::BUTTON, callback, texture),
		standby(standby),
		OnMouse(OnMouse),
		Tick(OnMouse),
		OnClick(OnClick),
		type(type)
	{
		this->text = new Text(text, position.x, position.y, font, color);
	}

	Button(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, button_type type) :
		position({ x, y }),
		texture(texture),
		standby(standby),
		Tick(Tick),
		OnClick(OnClick),
		type(type)
	{}

	~Button();

	Text* text = nullptr;
	iPoint position;
	SDL_Texture* texture = nullptr;
	SDL_Rect standby;
	SDL_Rect OnMouse;
	SDL_Rect Tick;
	SDL_Rect OnClick;
	button_state state = STANDBY;
	button_type type = LINK;
	bool tick = false;
	bool clicked = false;
};

#endif // !__UI_BUTTON__
