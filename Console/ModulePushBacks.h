#ifndef __ModulePushbacks_H__
#define __ModulePushbacks_H__

#define MAX_PUSHBACKS 30

#include "Module.h"
#include "ModuleCollision.h"

struct Pushback
{
	Collider* collider;
	int position;
	Pushback* next;
	Pushback* prev;
	SDL_Rect rect;
};

class ModulePushbacks : public Module
{
public:
	Pushback* pushbacks[MAX_PUSHBACKS];
	int anim_speed;
	bool anim_loop;
private:
	Pushback* first;
	Pushback* last;
	char* text;
};

#endif // !__ModulePushbacks_H__
