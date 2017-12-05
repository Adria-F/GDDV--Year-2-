#include "j1App.h"
#include "UI_Window.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "j1Gui.h"
#include "j1Render.h"

void Window::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;
	content.add(child);
	
}

void Window::appendChildAtCenter(UI_element * child)
{
	iPoint child_pos(section.w / 2, section.h / 2);
	child_pos.x -= child->section.w / 2;
	child_pos.y -= child->section.h / 2;
	child->localPosition = { child_pos.x, child_pos.y };
	child->parent = this;
	content.add(child);
}

void Window::BlitElement()
{
	absolutePosition = calculateAbsolutePosition();
	App->render->Blit(texture, absolutePosition.x + Displace.x , absolutePosition.y + Displace.y, &section, false);

	for (p2List_item<UI_element*>* item = content.start; item; item = item->next)
	{
		//if (item = focus)


		item->data->BlitElement();
	}
}
