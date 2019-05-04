#include "j1App.h"
#include "j1FowManager.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Render.h"
#include <assert.h>

j1FowManager::j1FowManager()
{
	name.assign("Fog of War Manager");
}

j1FowManager::~j1FowManager() {}

bool j1FowManager::Awake(pugi::xml_node& node)
{
	bool ret = true;

	return ret;
}

bool j1FowManager::Start()
{
	bool ret = true;

	smoothFogTex = App->tex->LoadTexture("textures/fow_textures3.png");

	// foggy tiles rects
	for (int i = 0; i < MAX_FOW_GRAPHICS; ++i)
		foggyTilesRects[i] = { i * 64, 0, 64,64 };

	// initialize table for get index of foggyTilesRects
	int i = 0;
	for (; i < NUM_FOW_ENTRIES; i++)
	{
		fog_rects_table[i] = -1;
	}

	// index 0 are reserved to totally shrouded/fogged tile
	// big corners

	fog_rects_table[0x5F] = 1; // corner NW
	fog_rects_table[0x137] = 2; // corner NE
	fog_rects_table[0x1D9] = 3;// corner SW
	fog_rects_table[0x1F4] = 4; // corner SE
	
	// straights
	fog_rects_table[0x7] = 5; // straight NNN
	fog_rects_table[0x1C0] = 6; // straight SSS
	fog_rects_table[0x124] = 7; // straight EEE
	fog_rects_table[0x49] = 8; // straight WWW

	// joints
	fog_rects_table[0x26] = 9; // joint NE
	fog_rects_table[0xB] = 10; // joint NW
	fog_rects_table[0x1A0] = 11; // joint SE
	fog_rects_table[0xC8] = 12; // joint SW


	//debug = true;

	return ret;
}

bool j1FowManager::PreUpdate()
{
	bool ret = true;

	// reset fog data
	for (int i = 0; i < width * height; ++i)
		App->fogOfWar->fogDataMap[i].m_bits_fog = fow_all;

	return ret;
}

bool j1FowManager::Update(float dt)
{
	bool ret = true;

	// update current emitters visibility
	for (std::list<FowEmitter*>::iterator iter = currentEmitters.begin(); iter != currentEmitters.end();)
	{
		if (!(*iter)->to_delete)
		{
			(*iter)->Update(dt);
			++iter;
		}
		else
		{
			(*iter)->CleanUp();
			delete (*iter);
			iter = currentEmitters.erase(iter);
		}
	}

	return ret;
}

bool j1FowManager::PostUpdate()
{
	bool ret = true;

	if (debug)
	{
		for (std::list<FowEmitter*>::iterator iter = currentEmitters.begin(); iter != currentEmitters.end(); ++iter)
		{
			(*iter)->PostUpdate();
		}
	}

	// TODO: print shroud and fogged areas, remember: m_bits_fog and m_bits_shroud of our fogDataMap array
	// only if we get a correct defined frame on our table fog_rects_table[]

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// gets our current x,y FowTile
			FOWTILE * tile = &fogDataMap[y * width + x];
			// get our frames id from fog data
			const int frame_id_fog = fog_rects_table[tile->m_bits_fog];
			// get our frames id from shroud data
			const int frame_id_shroud = fog_rects_table[tile->m_bits_shroud];

			// convert coords to world for print
			iPoint drawPos = App->map->MapToWorld(x, y);

			// draw fog
			if (frame_id_fog != -1)
			{
				SDL_SetTextureAlphaMod(smoothFogTex, 120);
				
				App->render->Blit(smoothFogTex, drawPos.x, drawPos.y, &foggyTilesRects[frame_id_fog]);
			}
			// draw shroud on top
			if (frame_id_shroud != -1)
			{
				SDL_SetTextureAlphaMod(smoothFogTex, 255);
				App->render->Blit(smoothFogTex, drawPos.x, drawPos.y, &foggyTilesRects[frame_id_shroud]);
			}
		}
	}

	return ret;
}

bool j1FowManager::CleanUp()
{
	bool ret = true;

	// "deletes" any possible current emitter
	if (!currentEmitters.empty())
	{
		for (std::list<FowEmitter*>::iterator iter = currentEmitters.begin(); iter != currentEmitters.end(); )
		{
			delete (*iter);
			iter = currentEmitters.erase(iter);
		}
	}

	// "deletes" fogDataMap
	RELEASE_ARRAY(fogDataMap);

	// unload textures
	if (smoothFogTex != nullptr)
		App->tex->UnloadTexture(smoothFogTex);

	return ret;
}

void j1FowManager::CreateFogDataMap(uint width, uint height)
{
	this->width = width;
	this->height = height;

	if (fogDataMap != nullptr)
	{
		RELEASE_ARRAY(fogDataMap);
	}

	fogDataMap = new FOWTILE[width * height];
	memset(fogDataMap, NULL, width*height);

	for (int i = 0; i < width*height; ++i)
	{
		fogDataMap[i].m_bits_fog = fow_all;
		fogDataMap[i].m_bits_shroud = fow_all;
	}
}

FOWTILE* j1FowManager::GetFogTileAt(iPoint position) const
{
	FOWTILE* ret = nullptr;

	if (CheckFogMapBoundaries(position))
	{
		ret = &fogDataMap[(position.y * width) + position.x];
	}

	return ret;
}

bool j1FowManager::CheckFogMapBoundaries(iPoint position) const
{
	return (position.x >= 0 && position.x < width &&
		position.y >= 0 && position.y < height);
}

FowEmitter* j1FowManager::AddFogEmitter(uint radius)
{
	FowEmitter* ret = nullptr;

	ret = new FowEmitter(radius);

	if (ret != nullptr)
		currentEmitters.push_back(ret);

	return ret;
}

bool j1FowManager::IsThisTileVisible(iPoint position) const
{
	bool ret = false;

	// convert coords
	iPoint tilePos = App->map->WorldToMap(position.x, position.y);

	if (CheckFogMapBoundaries(tilePos))
	{
		// TODO: check on fogged map bits
		FOWTILE* tile = GetFogTileAt(tilePos);

		if (tile != nullptr)
		{
			if (tile->m_bits_fog != fow_all) // warning, corners and joints too
				ret = true;
		}
	}

	return ret;
}



// FOG OF WAR EMITTER CLASS --------------------------------------------------------------

FowEmitter::FowEmitter(){}

FowEmitter::FowEmitter(uint radius) : radius(radius)
{
	// check radius protection
	assert(radius >= FOW_RADIUS_MIN);
	assert(radius <= FOW_RADIUS_MAX);

}

FowEmitter::~FowEmitter(){}

bool FowEmitter::Start()
{
	bool ret = true;

	return ret;
}

bool FowEmitter::Update(float dt)
{
	bool ret = true;

	// get potentially affected tiles on our "radius"
	std::list<iPoint> touchedPositions = GetTilesAffected();
	// apply the mask
	ApplyMaskToMap(touchedPositions);

		
	return ret;
}

void FowEmitter::ApplyMaskToMap(std::list<iPoint> tiles)
{
	// apply the mask
	// bitwise ANDded to fog data map

	const unsigned short *  mask = &App->fogOfWar->area_mask[radius - FOW_RADIUS_MIN][0];

	for (std::list<iPoint>::iterator iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		FOWTILE* tile = App->fogOfWar->GetFogTileAt((*iter));

		if (tile != nullptr)
		{
			tile->m_bits_fog &= *mask;
			tile->m_bits_shroud &= *mask;
		}

		mask++;
	}
	
}

std::list<iPoint> FowEmitter::GetTilesAffected() const
{
	int radius = this->radius;
	int xCenter = position.x;
	int yCenter = position.y;

	const int   length = (radius * 2) + 1;
	const int   num_entries = length * length;

	std::list<iPoint> touchedPositions;


	for (int y = yCenter - (length / 2);
		y <= (yCenter + (length / 2));
		y++)
	{
		for (int x = xCenter - (length / 2);
			x <= (xCenter + (length / 2));
			x++)
		{
			touchedPositions.push_back({ x,y });
		}
	}

	return touchedPositions;
}

bool FowEmitter::PostUpdate()
{
	bool ret = true;


	return ret;
}

bool FowEmitter::CleanUp()
{
	bool ret = true;

	return ret;
}

void FowEmitter::SetPos(iPoint position)
{
	this->position = App->map->WorldToMap(position.x, position.y);
}