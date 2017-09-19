#include "ModuleChat.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_timer.h"

Chat::Chat()
{

}

Chat::~Chat()
{

}

bool Chat::Start()
{
	box_height = 50;
	box_margin = 1;
	font_score = App->fonts->Load("assets/UI/font.png", "!º#$%&'()*+,-./:;<=>0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@?[]abcdefghijklmnopqrstuvwxyz{|}~`\^_¿      ", 10);
	chat.Text.text = nullptr;
	chat.Text.length = 0;
	chat.cursor_pos = 0;
	cursor_start = SDL_GetTicks();
	cursor = true;

	return true;
}

update_status Chat::Update()
{
	App->render->DrawQuad({ 0, SCREEN_HEIGHT - box_height, SCREEN_WIDTH, box_height }, 255, 255, 255, 255);
	App->render->DrawQuad({ box_margin, SCREEN_HEIGHT - box_height + box_margin, SCREEN_WIDTH - (2 * box_margin), box_height - (2 * box_margin) }, 0, 0, 0, 255);

	int mayus = 0;

	for (int i = 4; i < 30; i++)
	{
		if (App->input->keyboard[i] == KEY_DOWN)
		{
			if (App->input->keyboard[SDL_SCANCODE_LSHIFT] == KEY_REPEAT)
				mayus = 32;
			else
				mayus = 0;
			addChar(i + 93 - mayus, chat.Text);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		addChar(32, chat.Text);
	}

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN && chat.cursor_pos > 0)
		chat.cursor_pos--;
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN && chat.cursor_pos < chat.Text.length)
		chat.cursor_pos++;

	if (App->input->keyboard[SDL_SCANCODE_BACKSPACE] == KEY_DOWN)
	{
		if (chat.cursor_pos > 0)
		{
			char* tempText = new char[chat.Text.length--];
			int j = 0;
			for (int i = 0; i < chat.Text.length; i++)
			{
				if (i == chat.cursor_pos - 1)
					j++;

				tempText[i] = chat.Text.text[j];
				j++;
			}
			chat.Text.text = tempText;
			chat.cursor_pos--;
		}
	}

	if (chat.Text.text != nullptr)
		App->fonts->BlitText(10, SCREEN_HEIGHT - box_height + box_margin + 15, font_score, chat.Text.text);

	cursorBlink();

	return UPDATE_CONTINUE;
}

void Chat::cursorBlink()
{
	cursor_now = SDL_GetTicks() - cursor_start;
	if (cursor_now > cursor_delay && cursor)
	{
		cursor_start = SDL_GetTicks();
		cursor = false;
	}
	else if (cursor_now > cursor_delay || cursor)
	{
		if (!cursor)
		{
			cursor_start = SDL_GetTicks();
		}
		cursor = true;
		App->fonts->BlitText(5 + (chat.cursor_pos * 22), SCREEN_HEIGHT - box_height + box_margin + 15, font_score, "|");
	}
}

void Chat::addChar(int id, message& text)
{
	bool overwritten = false;
	char* tempText = new char[text.length++];
	int j = 0;
	for (int i = 0; i < chat.Text.length - 1 + ((overwritten)? 1 : 0); i++)
	{
		if (i == chat.cursor_pos)
		{
			tempText[j] = id;
			j++;
			overwritten = true;
		}
		tempText[j] = text.text[i];
		j++;
	}
	if (!overwritten)
		tempText[chat.Text.length - 1] = id;
	text.text = tempText;
	chat.cursor_pos++;
}

