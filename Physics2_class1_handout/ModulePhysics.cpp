#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"
#include <time.h>

#ifdef  _DEBUG
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")
#endif //  _DEBUG
#ifndef _DEBUG
#pragma comment(lib, "Box2D/libx86/Release/Box2D.lib")
#endif // !_DEBUG

#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL 0.02f

#define DEAD_MARGIN 50

#define PIXELS_TO_METERS(p) ((float) METER_PER_PIXEL * p)
#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))


// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	b2Vec2 gravity(0.0f, 10.0f);
	world = new b2World(gravity);


	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH / 2), PIXELS_TO_METERS(SCREEN_HEIGHT / 2));
	b2Body* body = world->CreateBody(&bodyDef);
	
	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(SCREEN_HEIGHT / 2.5);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	body->CreateFixture(&fixture);

	//Box
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH / 2), PIXELS_TO_METERS(((SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 2.5) + 5)));
	b2Body* body2 = world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(PIXELS_TO_METERS(((SCREEN_HEIGHT / 2.5) + 150)), PIXELS_TO_METERS(5));

	fixture.shape = &polygonShape;
	body2->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE))
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		b2Body* body = world->CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = PIXELS_TO_METERS((rand() % 25 + 5));

		b2FixtureDef fixture;
		fixture.shape = &shape;
		body->CreateFixture(&fixture);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	LOG("%d", world->GetBodyCount());
	for(b2Body* b = world->GetBodyList(); b; )
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
					break;
				}
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 size;
					size.x = (shape->GetVertex(1).x - shape->GetVertex(0).x);
					size.y = (shape->GetVertex(0).y - shape->GetVertex(3).y);					
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawQuad({ METERS_TO_PIXELS((pos.x - size.x/2)), METERS_TO_PIXELS((pos.y - size.y/2)), METERS_TO_PIXELS(size.x), METERS_TO_PIXELS(size.y) }, 255, 255, 255);
					break;
				}			

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
		
		b2Body* tempB = b;
		b = b->GetNext();
		b2Vec2 position = tempB->GetPosition();
		if (METERS_TO_PIXELS(position.x) < -DEAD_MARGIN || METERS_TO_PIXELS(position.x) > SCREEN_WIDTH + DEAD_MARGIN || METERS_TO_PIXELS(position.y) < -DEAD_MARGIN || METERS_TO_PIXELS(position.y) > SCREEN_HEIGHT + DEAD_MARGIN)
		{
			world->DestroyBody(tempB);
		}

	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete(world);

	return true;
}
