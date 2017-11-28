#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;
struct SDL_Texture;
class UI_element;

enum event_type
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK,
	MOUSE_RELEASE
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;
	UI_element* createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color = { 255, 255, 255, 255 }, j1Module* callback = nullptr);
	UI_element* createImage(int x, int y, SDL_Texture* texture, j1Module* callback = nullptr);
	UI_element* createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	UI_element* createButton(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback = nullptr);
	//NULL texture to use atlas
	UI_element* createCheckBox(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, j1Module* callback = nullptr);
	//NULL texture to use atlas
	UI_element* createInputText(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback = nullptr);

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UI_element*> UI_elements;


};

#endif // __j1GUI_H__