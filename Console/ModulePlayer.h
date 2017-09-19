#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "p2Point.h"
#include "ModuleCollision.h"
#include "Animation.h"

#define PLAYER_SPEED 2

struct SDL_Texture;

class Player : public Module
{
public:

	Player(int num);
	~Player();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

public:

	COLLIDER_TYPE collider_type;
	
	iPoint position;

	Collider* Pcollider;
	SDL_Texture* graphics = nullptr;
	Animation* animation;
	Animation up;
	Animation down;
	Animation right;
	Animation left;
	Animation idle;

	fPoint vector;
	float module;

	bool going_up;
	bool going_left;
	bool going_down;
	bool going_right;
	bool dead;

	uint points = 0;
	int lives = 3;
	int num = 0;
	fPoint collider_move;
};

#endif // !__MODULE_PLAYER_H__
