#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"

#define MAX_ACTIVE_PARTICLES 200
#define SMALL_SHOT_SPEED 2.0
#define BIG_SHOT_SPEED 2.0

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

enum particle_type
{

};

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	fPoint position;
	fPoint speed;
	Uint32 born = 0;
	bool to_delete = false;
	bool fx_played = false;
	particle_type type;
	bool going_up = true;
	int damage = 1;
	int life = 100000000;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	Particle* AddParticle(const Particle& particle, particle_type type, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);

public:



	Particle* active[MAX_ACTIVE_PARTICLES];
	fPoint vector;
	float modul;
	float y_phase;
	int distance[2];

public:

	
};

#endif // __MODULEPARTICLES_H__
