#ifndef __UI_INPUTBOX__
#define __UI_INPUTBOX__

#include "UI_element.h"
#include "UI_Text.h"

class InputBox : public UI_element
{
public:

	InputBox(char* defaultText, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback) : UI_element(x, y, element_type::INPUTTEXT, section, callback, texture),
		defaultText(defaultText)
	{
		appendChild(10, 13, new Text(defaultText, localPosition.x, localPosition.y, font, color, nullptr));
	}

	~InputBox();

	void readInput();
	void setOutlined(bool isOutlined);
	void appendChild(int x, int y, UI_element* child);
	void BlitElement();

public:

	Text* text = nullptr;
	p2SString defaultText;
	bool reading = false;
};

#endif // !__UI_INPUTBOX__