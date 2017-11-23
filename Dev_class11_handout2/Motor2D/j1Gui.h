#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;
struct SDL_Texture;

// TODO 1: Create your structure of classes
struct Text
{
	Text()
	{}

	Text(char* text, int x, int y, _TTF_Font* font, SDL_Color color) : text(text), position({x, y}), font(font), color(color)
	{}

	~Text();

	char* text = nullptr;
	_TTF_Font* font = nullptr;
	iPoint position;
	SDL_Color color;
	SDL_Texture* texture;

	void createTexture();
};

struct Image
{
	Image()
	{}

	Image(SDL_Texture* texture, int x, int y, SDL_Rect section): texture(texture), position({x, y}), section(section)
	{}

	~Image();

	iPoint position;
	SDL_Texture* texture = nullptr;
	SDL_Rect section;
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

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;
	Text* createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color = { 255, 255, 255, 255 });
	Image* createImage(int x, int y, SDL_Rect* section = NULL, SDL_Texture* texture = NULL);

private:

	void blitTexts();
	void blitImages();

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<Text*> texts;
	p2List<Image*> images;
};

#endif // __j1GUI_H__