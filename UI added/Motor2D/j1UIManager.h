#ifndef __j1UIMANAGER_H__
#define __j1UIMANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_pixels.h"
#include "SDL/include/SDL_rect.h"
#include "p2List.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;
struct SDL_Texture;

enum button_state
{
	STANDBY,
	MOUSEOVER,
	CLICKED
};

enum button_type
{
	LINK,
	CHECKBOX
};

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
	SDL_Texture* outline;
	uint tex_width;
	uint tex_height;
	iPoint outline_offset;

	void createTexture();
};

struct Image
{
	Image()
	{}

	Image(SDL_Texture* texture, int x, int y, SDL_Rect* section): texture(texture), position({x, y}), section(section)
	{}

	~Image();

	iPoint position;
	SDL_Texture* texture = nullptr;
	SDL_Rect* section = nullptr;
};

struct Button
{	
	Button()
	{}

	Button(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, button_type type) :
		position({ x, y }),
		texture(texture),
		standby(standby),
		OnMouse(OnMouse),
		Tick(OnMouse),
		OnClick(OnClick),
		type(type)
	{
		this->text = new Text(text, position.x, position.y, font, color);
	}

	Button(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, button_type type) :
		position({ x, y }),
		texture(texture),
		standby(standby),
		Tick(Tick),
		OnClick(OnClick),
		type(type)
	{}

	~Button();

	Text* text = nullptr;
	iPoint position;
	SDL_Texture* texture = nullptr;
	SDL_Rect standby;
	SDL_Rect OnMouse;
	SDL_Rect Tick;
	SDL_Rect OnClick;
	button_state state = STANDBY;
	button_type type = LINK;
	bool tick = false;
	bool clicked = false;
};

struct Slider
{
	Slider()
	{}

	Slider(int x, int y, SDL_Texture* texture, SDL_Rect bar, SDL_Rect container, SDL_Rect button):
		position({x, y}),
		texture(texture),
		bar(bar),
		container(container),
		button(button)
	{}

	~Slider();

	SDL_Texture* texture;
	SDL_Rect container;
	SDL_Rect bar;
	SDL_Rect button;
	iPoint position;
	int progress;

	void setProgress(int newProgress);
};

// ---------------------------------------------------
class j1UIManager : public j1Module
{
public:

	j1UIManager();

	// Destructor
	virtual ~j1UIManager();

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

	const SDL_Texture* GetAtlas() const;
	Text* createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color = { 255, 255, 255, 255 });
	Image* createImage(int x, int y, SDL_Texture* texture);
	Image* createImageFromAtlas(int x, int y, SDL_Rect section);
	//NULL texture to use atlas
	Button* createButton(char* text, _TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick);
	//NULL texture to use atlas
	Button* createCheckBox(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick);
	//NULL texture to use atlas
	Slider* createSlider(int x, int y, SDL_Texture* texture, SDL_Rect bar, SDL_Rect container, SDL_Rect button);

private:

	void blitTexts();
	void blitImages();
	void blitButtons();
	void blitSliders();

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<Text*> texts;
	p2List<Image*> images;
	p2List<Button*> buttons;
	p2List<Slider*> sliders;
};

#endif // __j1UIMANAGER_H__