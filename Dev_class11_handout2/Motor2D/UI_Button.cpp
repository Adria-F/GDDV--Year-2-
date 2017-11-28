#include "UI_Button.h"
#include "j1App.h"
#include "j1Render.h"

Button::~Button()
{
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void Button::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void Button::BlitElement()
{
	if (clicked)
		App->render->Blit(texture, position.x, position.y, &OnClick, false);
	else
		App->render->Blit(texture, position.x, position.y, &standby, false);
	if (type == CHECKBOX && tick)
	{
		App->render->Blit(texture, position.x, position.y, &Tick, false);
	}
	else if (type == LINK)
	{
		text->position.x = position.x + standby.w / 2 - text->tex_width / 2;
		text->position.y = position.y + standby.h / 2 - text->tex_height / 2;
		
		text->BlitElement();
	}
}
