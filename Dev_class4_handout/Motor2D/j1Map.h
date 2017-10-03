#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

#define MAX_TILESETS 10
#define MAX_LAYERS 10

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct tileset
{
	int firstGid;
	char* name;
	uint tileWidth;
	uint tileHeight;
	uint spacing;
	uint margin;
	char* imgPath;
	uint imgWidth;
	uint imgHeight;

};

struct layer
{
	char* name;
	int width;
	int height;
};

enum orientations
{
	orthogonal,
	isometric,
	staggered,
	hexagonal
};

enum order
{
	rightDown,
	rightUp,
	leftDown,
	leftUp
};
// TODO 1: Create a struct needed to hold the information to Map node
struct map
{
	orientations orientation;
	order renderOrder;
	uint width;
	uint height;
	uint tileWidth;
	uint tileHeight;
	int nextObjectId;
	int tilesets;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:

	bool LoadMap(pugi::xml_node& map);
	bool LoadTileSet(pugi::xml_node& node);
	bool LoadLayer(pugi::xml_node& node);
	bool setOrientation(char* orientation);

public:

	// TODO 1: Add your struct for map info as public for now
	map scene1;
	tileset* tileSets[MAX_TILESETS];
	layer* layers[MAX_LAYERS];

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__