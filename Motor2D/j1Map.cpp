//#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
//#include <math.h>
//#include "j1Scene.h"
#include "Brofiler/Brofiler.h"
#include "j1FowManager.h"

j1Map::j1Map() : j1Module() , map_loaded(false)
{
	name.assign("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.assign(config.child("folder").child_value());

	// create a list with total num of levels

	pugi::xml_node levelsNode = config.child("levels");
	if (levelsNode == NULL)
	{
		LOG("num of levels not found");
	}
	else
	{
		LOG("level data found");

		for (pugi::xml_node level = levelsNode.child("level"); level; level = level.next_sibling("level"))
		{
			Levels* newLevel = new Levels();
			newLevel->name = level.attribute("name").as_string();
			data.numLevels++;

			data.levels.push_back(newLevel);
		}
	}

	
	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("MAP DRAW", Profiler::Color::DeepPink);

	if(map_loaded == false)
		return;

	//int mapBlitOffset = 50;//TODO: Get this from the xml

	
	for (std::list<MapLayer*>::iterator layer = data.mapLayers.begin(); layer != data.mapLayers.end(); ++layer)
	{
		if ((*layer)->name.data() == "navigationLayer" && showNavLayer == false) {
			continue;
		}

		for (int y = 0; y < data.rows; ++y)
		{
			for (int x = 0; x < data.columns; ++x)
			{
				// TODO: CHECK IF WE HAVE SHROUD, if we have, ignore this print
				if (App->fogOfWar->GetFogTileAt({ x,y }) == FOGTYPE::SHROUD)
					continue;

				int tile_id = (*layer)->GetArrayPos(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);
						iPoint pos = MapToWorld(x, y);

						
							App->render->Blit(tileset->texture, pos.x, pos.y, &r, (*layer)->properties.parallaxSpeed);
						
					}
				}
			}
		}
	}

}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = *item;

	while (item != data.tilesets.end())
	{
		if (id < (*item)->firstgid)
		{
			set = *(--item);
			++item;
			break;
		}
		set = *item;
		++item;
	}

	return set;
}

iPoint j1Map::MapToWorld(int column, int row) const
{
	iPoint retVec(0,0);
	switch (data.type) {
	case MapTypes::MAPTYPE_ORTHOGONAL:
		retVec.x = column * data.tile_width;
		retVec.y = row * data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		retVec.x = (column - row) * data.tile_width * 0.5f;
		retVec.y = (column + row) * data.tile_height * 0.5f;
		break;
	default:
		LOG("ERROR: Map type not identified.");
		break;
	}
	
	return retVec;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint retVec(0,0);
	switch (data.type) {
	case MapTypes::MAPTYPE_ORTHOGONAL:
		retVec.x = x / data.tile_width;
		retVec.y = y / data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		retVec.y = y / data.tile_height - x / data.tile_width;
		retVec.x = (2 * x) / data.tile_width + retVec.y;
		break;
	default:
		LOG("ERROR: Map type not set.");
		break;
	}
	return retVec;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect = {0, 0, 0, 0};
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % columns));
	rect.y = margin + ((rect.h + spacing) * (relative_id / columns));
	return rect;
}

bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator tiles_item;
	tiles_item = data.tilesets.begin();

	while (tiles_item != data.tilesets.end())
	{
		RELEASE(*tiles_item);
		++tiles_item;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::iterator layer_item;
	layer_item = data.mapLayers.begin();

	while (layer_item != data.mapLayers.end())
	{
		data.mapLayers.remove(*layer_item);
		++layer_item;
	}
	data.mapLayers.clear();
	// Clean up the pugui tree
	map_file.reset();
	map_loaded = false;
	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	std::string tmp = ("%s%s", folder.data(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.data());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		// stores the current level string name
		data.loadedLevel.assign(file_name);
		// ------------------------------------
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.mapLayers.push_back(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.columns, data.rows);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		std::list<TileSet*>::iterator item = data.tilesets.begin();
		while(item != data.tilesets.end())
		{
			TileSet* s = (*item);
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.data(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			++item;
		}

		std::list<MapLayer*>::iterator item_layer = data.mapLayers.begin();
		while(item_layer != data.mapLayers.end())
		{
			MapLayer* l = (*item_layer);
			LOG("Layer ----");
			LOG("name: %s", l->name.data());
			LOG("tile width: %d tile height: %d", l->columns, l->rows);
			++item_layer;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.columns = map.attribute("width").as_int();
		data.rows = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.length() > 0)
		{
			std::string red, green, blue;
			red = (1, 2);
			green = (3, 4);
			blue = (5, 6);

			int v = 0;

			sscanf_s(red.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MapTypes::MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MapTypes::MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MapTypes::MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MapTypes::MAPTYPE_UNKNOWN;
		}

		// load custom properties from map data
		pugi::xml_node propertiesNode = map.child("properties");

		if (propertiesNode == NULL)
		{
			LOG("Map without custom properties");
		}
		else
		{
			LoadProperties(map, data.properties);
		}

	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	set->name.assign(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();

	pugi::xml_node offset = tileset_node.child("tileoffset");
	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->LoadTexture(PATH(folder.data(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->columns = set->tex_width / set->tile_width;
		set->rows = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->columns = node.attribute("width").as_int();
	layer->rows = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->tileArray = new uint[layer->columns*layer->rows];
		memset(layer->tileArray, 0, layer->columns*layer->rows);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->tileArray[i++] = tile.attribute("gid").as_int(0);
		}
	}

	// check custom properties from layer - test functionality // TODO, implement latest handout to do this
	/*pugi::xml_node properties = node.child("properties");
	if (properties == NULL)
	{
		LOG("This layer doesnt have custom properties defined");
	}
	else
	{
		layer->parallaxSpeed = properties.child("property").attribute("value").as_float();
		LOG("%f", layer->parallaxSpeed);
	}*/

	return ret;
}


bool j1Map::Reset()
{
	map_loaded = false;
	// TODO: maybe we need to search a less ugly workaround to restart scene
	//App->scene->Disable();
	//App->scene->Enable();

	if (CleanUp()) {
		return true;
	}

	return false;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	pugi::xml_node propertiesNode = node.child("properties");

	if (propertiesNode == NULL)
	{
		LOG("properties not found");
		ret = false;
	}
	else
	{
		properties.draw = propertiesNode.find_child_by_attribute("name", "Draw").attribute("value").as_bool(true);
		properties.navigation = propertiesNode.find_child_by_attribute("name", "Navigation").attribute("value").as_bool(false);
		properties.testValue = propertiesNode.find_child_by_attribute("name", "testValue").attribute("value").as_int(0);
		properties.parallaxSpeed = propertiesNode.find_child_by_attribute("name", "parallaxSpeed").attribute("value").as_float(1.0F);
		properties.music_name = propertiesNode.find_child_by_attribute("name", "background_music").attribute("value").as_string();
		//properties.fx_name = propertiesNode.find_child_by_attribute("name", "background_music").attribute("value").as_string();
		
		// associated gui xml with this map ----
		properties.gui_xml_path = propertiesNode.find_child_by_attribute("name", "associated_gui_xml").attribute("value").as_string();
	}

	return ret;
}


int j1Map::GetTileWidth() const
{
	return data.tile_width;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	std::list<MapLayer*>::const_iterator item = data.mapLayers.begin();

	for (; item != data.mapLayers.end(); ++item)
	{
		MapLayer* layer = (*item);

		if (layer->properties.navigation == false)
			continue;

		uchar* map = new uchar[layer->columns*layer->rows];
		memset(map, 1, layer->columns*layer->rows);

		for (int y = 0; y < data.rows; ++y)
		{
			for (int x = 0; x < data.columns; ++x)
			{
				int i = (y*layer->columns) + x;

				int tile_id = layer->GetArrayPos(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 1 : 0;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.columns;
		height = data.rows;
		ret = true;

		break;
	}

	return ret;
}