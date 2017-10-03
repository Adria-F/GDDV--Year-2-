#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
	for (int i = 0; i < MAX_TILESETS; i++)
		tileSets[i] = nullptr;
}

// Destructor
j1Map::~j1Map()
{
	for (int i = 0; i < MAX_TILESETS; i++)
	{
		if (tileSets[i] != nullptr)
		{
			delete tileSets[i];
			tileSets[i] = nullptr;
		}
	}
}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	for (int i = 0; i < MAX_TILESETS; i++)
	{
		if (tileSets[i] != nullptr)
		{
			App->render->Blit(App->tex->Load(tileSets[i]->imgPath), 0, 0);
		}
	}

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		ret = LoadMap(map_file.child("map"));
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	for (pugi::xml_node tileset = map_file.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
	{
		if (!LoadTileSet(tileset))
		{
			LOG("All tilesets full");
			ret = false;
		}
	}

	//Load all layers
	for (pugi::xml_node layer = map_file.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		if (!LoadLayer(layer))
		{
			LOG("All layers full");
			ret = false;
		}
	}

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", scene1.width, scene1.height);
		LOG("tile_width: %d tile_height: %d", scene1.tileWidth, scene1.tileHeight);
		for (int i = 0; i < MAX_TILESETS; i++)
		{
			if (tileSets[i] != nullptr)
			{
				LOG("Tileset ----");
				LOG("name: %s firstgid: %d", tileSets[i]->name, tileSets[i]->firstGid);
				LOG("tile width: %d tile height: %d", tileSets[i]->tileWidth, tileSets[i]->tileHeight);
				LOG("spacing: %d margin: %d", tileSets[i]->spacing, tileSets[i]->margin);
			}
		}
		for (int i = 0; i < MAX_LAYERS; i++)
		{
			if (layers[i] != nullptr)
			{
				LOG("Layer ----");
				LOG("name: %s", layers[i]->name);
				LOG("layer width: %d layer height: %d", layers[i]->width, layers[i]->height);
			}
		}
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap(pugi::xml_node& map)
{
	bool ret = true;

	if (!setOrientation((char*)map.attribute("orientation").as_string()))
	{
		LOG("Could not find orientation");
		ret = false;
	}

	scene1.width = map.attribute("width").as_uint();
	scene1.height = map.attribute("height").as_uint();
	scene1.tileWidth = map.attribute("tilewidth").as_uint();
	scene1.tileHeight = map.attribute("tileheight").as_uint();
	scene1.nextObjectId = map.attribute("nextobjectid").as_int();
	scene1.tilesets = 0;

	return ret;
}

bool j1Map::LoadTileSet(pugi::xml_node& node)
{
	bool ret = false;

	for (int i = 0; i < MAX_TILESETS; i++)
	{
		if (tileSets[i] == nullptr)
		{
			tileSets[i] = new tileset;

			tileSets[i]->firstGid = node.attribute("firstgid").as_int();
			tileSets[i]->name = (char*)node.attribute("name").as_string();
			tileSets[i]->tileWidth = node.attribute("tilewidth").as_int();
			tileSets[i]->tileHeight = node.attribute("tileheight").as_int();
			tileSets[i]->spacing = node.attribute("spacing").as_int();
			tileSets[i]->margin = node.attribute("margin").as_int();
			tileSets[i]->imgPath = (char*)node.child("image").attribute("source").as_string();
			tileSets[i]->imgWidth = node.child("image").attribute("width").as_int();
			tileSets[i]->imgHeight = node.child("image").attribute("height").as_int();

			scene1.tilesets++;

			ret = true;
			break;
		}
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node)
{
	bool ret = false;

	for (int i = 0; i < MAX_LAYERS; i++)
	{
		if (layers[i] == nullptr)
		{
			layers[i] = new layer;
			layers[i]->name = (char*)node.attribute("name").as_string();
			layers[i]->width = node.attribute("width").as_int();
			layers[i]->height = node.attribute("height").as_int();

			ret = true;
			break;
		}
	}

	return ret;
}

bool j1Map::setOrientation(char* orientation)
{
	bool ret = true;

	if (strcmp(orientation, "orthogonal") == 0)
	{
		scene1.orientation = orthogonal;
	}
	else if (strcmp(orientation, "isometric") == 0)
	{
		scene1.orientation = isometric;
	}
	else if (strcmp(orientation, "staggered") == 0)
	{
		scene1.orientation = staggered;
	}
	else if (strcmp(orientation, "hexagonal") == 0)
	{
		scene1.orientation = hexagonal;
	}
	else
	{
		ret = false;
	}

	return ret;
}
