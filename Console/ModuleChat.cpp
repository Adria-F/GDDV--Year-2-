#include "ModuleChat.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_keycode.h"
#include "SDL/include/SDL_keyboard.h"

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
	bool cap_lock = false;

	int temp = SDL_GetModState();
	temp = temp & KMOD_CAPS;
	if (temp == KMOD_CAPS)
		cap_lock = true;

	for (int i = 30; i < 40; i++) //0-9
	{
		if (App->input->keyboard[i] == KEY_DOWN)
		{
			mayus = 0;
			if (i == 39)
				mayus = 10;
			if (App->input->keyboard[SDL_SCANCODE_LSHIFT] == KEY_REPEAT && ((i >= 30 && i <= 38))) //symbols
			{
				mayus = 16;
				if (i == 31)
					mayus = 11;
				if (i == 36)
					mayus = 8;
			}
			addChar(i + 19 - mayus, chat.Text);
		}
	}

	for (int i = 4; i < 30; i++) //a-z
	{
		if (App->input->keyboard[i] == KEY_DOWN)
		{
			if (App->input->keyboard[SDL_SCANCODE_LSHIFT] == KEY_REPEAT) //A-Z
				mayus = (cap_lock)? 0 : 32;
			else
				mayus = (cap_lock) ? 32 : 0;
			addChar(i + 93 - mayus, chat.Text);
		}
	}

	//Un-ordered symbols
	{
		if (App->input->keyboard[SDL_SCANCODE_KP_PLUS] == KEY_DOWN || App->input->keyboard[48] == KEY_DOWN)
		{
			addChar(43, chat.Text);
		}
		if (App->input->keyboard[SDL_SCANCODE_KP_MINUS] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_SLASH] == KEY_DOWN)
		{
			addChar(45, chat.Text);
		}
		if (App->input->keyboard[SDL_SCANCODE_KP_MULTIPLY] == KEY_DOWN)
		{
			addChar(42, chat.Text);
		}
		if (App->input->keyboard[SDL_SCANCODE_COMMA] == KEY_DOWN)
		{
			addChar(44, chat.Text);
		}
		if (App->input->keyboard[SDL_SCANCODE_PERIOD] == KEY_DOWN)
		{
			addChar(46, chat.Text);
		}
		if (App->input->keyboard[84] == KEY_DOWN) // /
		{
			addChar(47, chat.Text);
		}
		for (int i = 0; i < 150; i++)
		{
			if (App->input->keyboard[i] == KEY_DOWN)
				LOG("KEY PRESSED: %d", i);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		addChar(32, chat.Text);
	}

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN && chat.cursor_pos > 0)
		chat.cursor_pos--;
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN && chat.cursor_pos < chat.Text.text.length())
		chat.cursor_pos++;

	if (App->input->keyboard[SDL_SCANCODE_BACKSPACE] == KEY_DOWN)
	{
		if (chat.cursor_pos > 0)
		{
			chat.Text.text.erase(chat.cursor_pos - 1, 1);
			chat.cursor_pos--;
		}
	}

	App->fonts->BlitText(10, SCREEN_HEIGHT - box_height + box_margin + 15, font_score, chat.Text.text.c_str());

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
	if (chat.cursor_pos != chat.Text.text.length())
	{
		chat.Text.text.insert(chat.cursor_pos, 1, id);
	}
	else
	{
		chat.Text.text += id;
	}
	chat.cursor_pos++;
}

