#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"

Image::~Image()
{
	if (section != nullptr)
	{
		delete section;
		section = nullptr;
	}
}

void Image::BlitElement()
{
	App->render->Blit(texture, position.x, position.y, section, false);
}
