#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	LOG("%d", world->GetBodyCount());

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position
	

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	int x, y;
	for (int i = 0; i < MAX_CIRCLES; i++)
	{
		if (circles[i] != nullptr)
		{
			circles[i]->getPosition(&x, &y);
			if (x < -DEAD_MARGIN || x > SCREEN_WIDTH + DEAD_MARGIN || y < -DEAD_MARGIN || y > SCREEN_HEIGHT + DEAD_MARGIN)
			{
				world->DestroyBody(circles[i]->getBody());
				circles[i] = nullptr;
			}

		}
	}
	for (int i = 0; i < MAX_RECTANGLES; i++)
	{
		if (rectangles[i] != nullptr)
		{
			rectangles[i]->getPosition(&x, &y);
			if (x < -DEAD_MARGIN || x > SCREEN_WIDTH + DEAD_MARGIN || y < -DEAD_MARGIN || y > SCREEN_HEIGHT + DEAD_MARGIN)
			{
				world->DestroyBody(rectangles[i]->getBody());
				rectangles[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < MAX_CHAINS; i++)
	{
		if (chains[i] != nullptr)
		{
			chains[i]->getPosition(&x, &y);
			if (x < -DEAD_MARGIN || x > SCREEN_WIDTH + DEAD_MARGIN || y < -DEAD_MARGIN || y > SCREEN_HEIGHT + DEAD_MARGIN)
			{
				world->DestroyBody(chains[i]->getBody());
				chains[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	return new PhysBody(b);
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int w, int h)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	shape.SetAsBox(PIXEL_TO_METERS(w), PIXEL_TO_METERS(h));
	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	return new PhysBody(b);
}

PhysBody* ModulePhysics::CreateChain(int x, int y, float size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	// Pivot 0, 0
	int rick_head[82] = {
		99, 127,
		96, 136,
		90, 145,
		78, 149,
		64, 147,
		49, 139,
		46, 133,
		35, 137,
		40, 126,
		24, 125,
		32, 116,
		10, 103,
		29, 91,
		0, 76,
		30, 63,
		14, 36,
		42, 39,
		44, 23,
		41, 0,
		64, 19,
		76, 33,
		83, 20,
		88, 5,
		93, 21,
		95, 40,
		112, 36,
		103, 59,
		105, 61,
		105, 64,
		117, 67,
		109, 74,
		110, 84,
		108, 91,
		104, 94,
		110, 100,
		103, 106,
		100, 116,
		104, 120,
		106, 123,
		103, 126,
		100, 126
	};
	b2Vec2 vertexs[41];
	for (int i = 0; i < 41; i++)
	{
		vertexs[i] = { PIXEL_TO_METERS((rick_head[2 * i] * size)), PIXEL_TO_METERS((rick_head[1 + 2 * i] * size)) };
	}
	shape.CreateLoop(vertexs, 41);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	return new PhysBody(b);
}

void PhysBody::getPosition(int* x, int* y)
{
	*x = METERS_TO_PIXELS(body->GetPosition().x);
	*y = METERS_TO_PIXELS(body->GetPosition().y);
}

void PhysBody::getRadius(int* r)
{
	*r = METERS_TO_PIXELS(body->GetFixtureList()->GetShape()->m_radius);
}

void PhysBody::getSize(int* w, int* h)
{
	b2PolygonShape* shape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
	*w = METERS_TO_PIXELS(shape->m_vertices[1].x - shape->m_vertices[0].x);
	*h = METERS_TO_PIXELS(shape->m_vertices[3].y - shape->m_vertices[0].y);
}

void PhysBody::getRotation(float* d)
{
	*d = body->GetAngle() * RADTODEG;
}