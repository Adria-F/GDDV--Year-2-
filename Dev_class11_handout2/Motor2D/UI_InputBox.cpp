#include "UI_InputBox.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "SDL\include\SDL_events.h"

InputBox::~InputBox()
{
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void InputBox::readInput()
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

void InputBox::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void InputBox::BlitElement()
{
	App->render->Blit(texture, position.x, position.y, &box, false);

	if (text != nullptr)
	{
		text->position.x = position.x + box.w / 2 - text->tex_width / 2;
		text->position.y = position.y + box.h / 2 - text->tex_height / 2;
		
		text->BlitElement();
	}
}
