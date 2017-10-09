#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "j1Collision.h"
#include "p2Point.h"

#define PLAYER_SPEED 2

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	COLLIDER_TYPE collider_type;

	Animation jumping_right;

	Collider* Pcollider;
	Collider* collidingC;

	float top_speed = 0;

	bool jumping;
	bool going_left;
	bool going_down;
	bool going_right;
	bool dead;
	bool falling;

	uint points = 0;
	int lives = 3;
	int num = 0;
	fPoint collider_move;
};

#endif