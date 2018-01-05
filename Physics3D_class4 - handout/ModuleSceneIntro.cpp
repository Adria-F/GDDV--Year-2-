#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include <time.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (int i = 0; i < MAX_SNAKE; i++)
	{
		pb_snake[i] = nullptr;
		pb_snake2[i] = nullptr;
	}

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// TODO 2: Chain few N spheres together to form a snake
	Sphere sphere(0.2f);
	float radius = 0;
	srand(time(NULL));

	for (int i = 0; i < MAX_SNAKE; ++i)
	{
		int colorR = rand() % 255;
		sphere.color = Color(colorR, 0, 0);
		radius += 0.2f;
		sphere.radius = radius;
		sphere.SetPos(sphere.transform.translation().x + 2 + radius, 0, 0);
		s_snake[i] = sphere;
		pb_snake[i] = App->physics->AddBody(sphere);
	}
	for (int i = 0; i < MAX_SNAKE-1; i++)
	{
		App->physics->AddConstraint(*pb_snake[i], *pb_snake[i+1], { s_snake[i].radius + 0.1f, 0, 0 }, { -s_snake[i+1].radius - 0.1f, 0, 0 });
	}

	// TODO 4: Chain few N spheres together to form a a bike's sphere
	// Be sure to put the right axis

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for(int i = 0; i < MAX_SNAKE; ++i)
	{
		if (pb_snake[i] != nullptr)
		{
			pb_snake[i]->GetTransform(&(s_snake[i].transform));
			s_snake[i].Render();
		}
	}

	for(int i = 0; i < MAX_SNAKE; ++i)
	{
		if (pb_snake2[i] != nullptr)
		{
			pb_snake2[i]->GetTransform(&(s_snake2[i].transform));
			s_snake2[i].Render();
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

