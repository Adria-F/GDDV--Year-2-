#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Render.h"

#define gravity -0.0025 // -0.1

struct SDL_Texture;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	DEAD,
	DAMAGED
};

class Entity : public j1Module
{
public:

	Entity()
	{
		a.x = 0;
		a.y = 0;
		v.x = 0;
		v.y = 0;
		Fa = 0;
	}

	bool Entity_Update()
	{
		v.y += (gravity * ((colliding_bottom) ? 0 : 1));
		if (v.y < -0.7)
			v.y = -0.7;
		position.y -= v.y;

		if (a.x > 0)
			Fa = gravity * 0.01;
		else if (a.x < 0)
			Fa = -gravity * 0.01;
		else if (v.x = 0)
			Fa = 0;

		if (a.x > 0)
			v.x += (a.x * ((colliding_right) ? 0 : 1)) + (Fa* ((colliding_right) ? 0 : 1));
		else if (a.x < 0 && position.x + App->render->camera.x > App->render->camera.x)
			v.x += (a.x * ((colliding_left) ? 0 : 1)) + (Fa* ((colliding_left) ? 0 : 1));
		else
			v.x = 0;

		position.x += v.x;

		if (v.x > 0.1)
			v.x = 0.1;
		else if (v.x < -0.1)
			v.x = -0.1;

		return true;
	}

public:
	fPoint position;
	fPoint a;
	fPoint v;
	float Fa;
	bool colliding_bottom;
	bool colliding_top;
	bool colliding_right;
	bool colliding_left;
	entity_state state;
	SDL_Texture* graphics = nullptr;
	Animation* animation;
	Animation idle_right;
	Animation idle_left;
	Animation right;
	Animation left;
	Animation jumping_left;
	Animation death;
};

#endif // !__ModuleEntity_H__
