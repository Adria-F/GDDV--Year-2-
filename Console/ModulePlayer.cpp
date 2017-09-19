#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_timer.h"

Player::Player(int num): num(num)
{

}

Player::~Player()
{
}

bool Player::Start()
{
	if (num == 1 && Pcollider == nullptr)
		Pcollider = App->collision->AddCollider({ 0, 0, 20, 24 }, COLLIDER_PLAYER, this);

	graphics = App->textures->Load("assets/characters/link.png");
	down.PushBack({ 1, 3, 19, 25 });
	up.PushBack({ 22, 3, 21 , 24 });
	right.PushBack({ 46, 4, 19, 23 });
	left.PushBack({ 71, 4, 19, 23 });
	animation = &down;
	collider_move.x = 0;
	collider_move.y = 0;

	return true;
}

bool Player::CleanUp()
{
	if (Pcollider != nullptr)
		Pcollider->to_delete = true;
	return true;
}

update_status Player::Update()
{
	if (going_up)
	{
		animation = &up;
		if (position.y > 45)
			position.y -= PLAYER_SPEED;
	}
	else if (going_down)
	{
		animation = &down;
		if (position.y < 460)
			position.y += PLAYER_SPEED;
	}
	else if (going_left)
	{
		animation = &left;
		if (position.x > 10)
			position.x -= PLAYER_SPEED;
	}
	else if (going_right)
	{
		animation = &right;
		if (position.x < 690)
			position.x += PLAYER_SPEED;
	}
	
	// Delete mouse collider when position reached
	if (App->input->mouse_collider != nullptr && (abs(App->input->mouse_collider->rect.x - position.x) < 2) && (abs(App->input->mouse_collider->rect.y - position.y) < 2))
	{
		App->input->mouse_collider->to_delete = true;
		App->input->mouse_collider = nullptr;
	}

	if (num == 1)
		Pcollider->SetPos(position.x + collider_move.x, position.y + collider_move.y); //x + 1 | y + 17

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());
	
	return UPDATE_CONTINUE;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_WALL)
	{
		if ((c2->rect.y + c2->rect.h) > c1->rect.y && going_up)
		{
			position.y += PLAYER_SPEED;
			going_up = false;
		}
		else if (c2->rect.y < (c1->rect.y + c1->rect.h) && going_down)
		{
			position.y -= PLAYER_SPEED;
			going_down = false;
		}
		else if ((c2->rect.x - c2->rect.w) < c1->rect.x && going_left)
		{
			position.x += PLAYER_SPEED;
			going_left = false;
		}
		else if (c2->rect.x > c1->rect.x && going_right)
		{
			position.x -= PLAYER_SPEED;
			going_right = false;
		}
	}
}