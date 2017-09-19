#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"

ModuleCollision::ModuleCollision()
{
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALL][COLLIDER_MOUSE] = false;
	matrix[COLLIDER_WALL][COLLIDER_CORNER] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_MOUSE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_CORNER] = false;

	matrix[COLLIDER_MOUSE][COLLIDER_WALL] = true;
	matrix[COLLIDER_MOUSE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_MOUSE][COLLIDER_MOUSE] = false;
	matrix[COLLIDER_MOUSE][COLLIDER_CORNER] = true;

	matrix[COLLIDER_CORNER][COLLIDER_WALL] = false;
	matrix[COLLIDER_CORNER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_CORNER][COLLIDER_MOUSE] = false;
	matrix[COLLIDER_CORNER][COLLIDER_CORNER] = false;

}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleCollision::Update()
{
	if (App->input->mouse[SDL_BUTTON_LEFT] == BUTTON_DOWN || App->input->mouse[SDL_BUTTON_RIGHT] == BUTTON_DOWN)
	{
		if (App->input->mouse_collider != nullptr)
		{
			App->input->mouse_collider->to_delete = true;
			App->input->mouse_collider = nullptr;
		}
		if (App->input->mouse[SDL_BUTTON_LEFT] == BUTTON_DOWN)
		{

		}
	}



	//------------------------------------------

	Collider* c1;
	Collider* c2;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if(colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		c1->Update();

		// avoid checking collisions already checked
		for(uint k = i+1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if(colliders[k] == nullptr)
				continue;

			c2 = colliders[k];
			c2->Update();

			if(c1->CheckCollision(c2->rect) == true)
			{
 				if(matrix[c1->type][c2->type] && c1->callback) 
					c1->callback->OnCollision(c1, c2);
				
				if(matrix[c2->type][c1->type] && c2->callback) 
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	if(App->render->debugging == false)
		return;

	Uint8 alpha = 80;
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
			continue;
		
		switch(colliders[i]->type)
		{
			case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
			case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, 0);
			break;
			case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
			case COLLIDER_MOUSE: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, 255);
			break;
		}
	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback, Particle* part)
{
	Collider* ret = nullptr;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback, part);
			if (type == COLLIDER_WALL)
			{
				for (int i = rect.y; i < (rect.y + rect.h); i++)
				{
					for (int j = rect.x; j < (rect.x + rect.w); j++)
					{
						
					}
				}
			}
			break;
		}
	}

	return ret;
}

bool ModuleCollision::EraseCollider(Collider* collider)
{
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool result = false;

	if (((rect.y + rect.h) > r.y && (r.y + r.h) > rect.y) && ((rect.x + rect.w) > r.x && (r.x + r.w) > rect.x))
	{
		result = true;
	}

	return result;
}

update_status Collider::Update()
{

	return UPDATE_CONTINUE;
}