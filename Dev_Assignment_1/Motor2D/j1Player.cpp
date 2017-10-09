#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"

#include<stdio.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

j1Player::j1Player()
{
	idle_right.PushBack({ 0, 66, 21, 33 });
	idle_left.PushBack({ 84, 66, 21, 33 });

	jumping_right.PushBack({ 63, 33, 21, 33 });
	jumping_left.PushBack({ 63, 132, 21, 33 });

	right.PushBack({ 0, 0, 21, 33 });
	right.PushBack({ 21, 0, 21, 33 });
	right.PushBack({ 42, 0, 21, 33 });
	right.PushBack({ 63, 0, 21, 33 });
	right.PushBack({ 84, 0, 21, 33 });
	right.PushBack({ 0, 33, 21, 33 });
	right.PushBack({ 21, 33, 21, 33 });
	right.PushBack({ 42, 33, 21, 33 });
	right.speed = 0.01f;

	left.PushBack({ 0, 99, 21, 33 });
	left.PushBack({ 21, 99, 21, 33 });
	left.PushBack({ 42, 99, 21, 33 });
	left.PushBack({ 63, 99, 21, 33 });
	left.PushBack({ 84, 99, 21, 33 });
	left.PushBack({ 0, 132, 21, 33 });
	left.PushBack({ 21, 132, 21, 33 });
	left.PushBack({ 42, 132, 21, 33 });
	left.speed = 0.01f;
	
	animation = &right;
	collider_move.x = 2;
	collider_move.y = 0;
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("textures/Character spritesheet.png");

	if (Pcollider == nullptr)
		Pcollider = App->collision->AddCollider({ 0, 0, 12, 16 }, COLLIDER_PLAYER, this);

	animation = &idle_right;

	position.y = 150;

	position.x = 0;

	dead = false;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	if (Pcollider != nullptr)
	{
		Pcollider->to_delete = true;
		Pcollider = nullptr;
	}

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	Entity_Update();

	if (jumping)
	{
		if (state != JUMPING)
		{
			v.y = 0.7; //3.2
			state = JUMPING;
		}
		if (animation == &right || animation == &idle_right)
		{
			animation = &jumping_right;
		}
		else if (animation == &left || animation == &idle_left)
		{
			animation = &jumping_left;
		}
	}
	if (going_left)
	{
		a.x = -0.1;
		if (animation == &jumping_right)
			animation = &jumping_left;
		else if (animation != &jumping_left)
		{
			animation = &left;
			state = LEFT;
		}
	}
	if (going_right)
	{
		a.x = 0.1;
		if (animation == &jumping_left)
			animation = &jumping_right;
		else if (animation != &jumping_right)
		{
			animation = &right;
			state = RIGHT;
		}
	}
	if (going_left == false && going_right == false)
	{
		a.x = 0;
	}

	if (going_down)
	{
		going_down = false;
		position.y += 1;
	}

	Pcollider->SetPos(position.x + collider_move.x + (App->render->camera.x/2), position.y + collider_move.y);

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(), 1.0, false);

	App->player->colliding_left = false;
	App->player->colliding_right = false;

	if (v.y < top_speed)
		top_speed = v.y;

	LOG("Player: %f", top_speed);

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if ((c2->rect.x + 4) > (c1->rect.x + (c1->rect.w)))
	{ 
		//LOG("Collision from right");
		//going_right = false;
		if (v.x > 0)
		{
			v.x = 0;
		}
		colliding_right = true;
	}
	else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 5))
	{ 
		//LOG("Collision from left");
		//going_left = false;
		if (v.x < 0)
		{
			v.x = 0;
		}
		colliding_left = true;
	}
	if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 4))
	{ 
		//LOG("Collision from top");
		if (v.y > 0)
		{
			v.y = 0;
		}
		//position.y += 1;
		colliding_top = true;
		collidingC = c2;
	}
	if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h))))
	{ 
		//LOG("Collision from bottom");
		if (colliding_bottom == false)
		{
			v.y = 0;
			state = IDLE;
			colliding_bottom = true;
		}
		if (animation == &jumping_left)
			animation = &idle_left;
		else if (animation == &jumping_right)
			animation = &idle_right;
		collidingC = c2;
	}
}