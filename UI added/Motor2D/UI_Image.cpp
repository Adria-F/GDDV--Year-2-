#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"

void Image::BlitElement()
{
	if (section.w != 0 && section.h != 0)
		App->render->Blit(texture, position.x, position.y, &section, false);
	else
		App->render->Blit(texture, position.x, position.y, nullptr, false);
}
