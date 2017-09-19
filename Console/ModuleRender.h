#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Renderer;
struct SDL_Texture;

class ModuleRender : public Module
{
public:
	ModuleRender(char* spritesheet);
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status Update();
	update_status PreUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	struct
	{
		float x;
		float y;
	}camera;

public:
	SDL_Renderer* renderer = nullptr;
	bool moving_scene = false;
	float cam_speed = 0.3f;
	bool debugging = true;
	bool god_mode = false;
	SDL_Texture* spritesheet_texture;
	char* spritesheet_path;
	uint screen_w;
	uint screen_h;

};

#endif //__ModuleRenderer_H__