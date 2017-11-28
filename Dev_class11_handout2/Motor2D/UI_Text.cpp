#include "UI_Text.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Fonts.h"

Text::~Text()
{
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}
}

void Text::createTexture()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}

	uint outline_width, outline_height;
	App->font->setFontOutline(font, 2);
	outline = App->font->Print(text.GetString(), { 0, 0, 0, 255 }, font); //Outlined texture
	App->tex->GetSize(outline, outline_width, outline_height);

	App->font->setFontOutline(font, 0);
	texture = App->font->Print(text.GetString(), color, font); //Normal texture
	App->tex->GetSize(texture, tex_width, tex_height);

	outline_offset.x = tex_width - outline_width;
	outline_offset.x /= 2;
	outline_offset.y = outline_offset.x;

}