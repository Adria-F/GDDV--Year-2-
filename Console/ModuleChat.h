#ifndef __MODULECHAT_H__
#define __MODULE_CHAT_H__

#include "Module.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include <iostream>
using namespace std;

enum textType
{
	NORMAL,
	COMMAND
};

struct message
{
	//int font_score = -1;
	int line_Y;
	string text; //*
	textType type;
};

struct chatBox
{
	Collider* box;
	int cursor_pos = 0;
	message Text;
};

class Chat : public Module
{
private:

	float cursor_delay = 500.0f;
	float cursor_start;
	float cursor_now;
	bool cursor;

	int font_score;
	int box_height;
	int box_margin;

	chatBox chat;

private:

	void cursorBlink();
	void addChar(int id, message& text);

public:

	Chat();
	~Chat();

	bool Start();
	update_status Update();
};

#endif // !__MODULECHAT_H__
