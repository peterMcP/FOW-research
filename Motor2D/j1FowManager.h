#ifndef __J1FOWMANAGER_H__
#define __J1FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include <list>
#include <vector>
#include <queue>

#define FOW_BIT_NW  (1 << 0)
#define FOW_BIT_N   (1 << 1)
#define FOW_BIT_NE  (1 << 2)
#define FOW_BIT_W   (1 << 3)
#define FOW_BIT_C   (1 << 4)
#define FOW_BIT_E   (1 << 5)
#define FOW_BIT_SW  (1 << 6)
#define FOW_BIT_S   (1 << 7)
#define FOW_BIT_SE  (1 << 8)

#define fow_non 0

#define fow_all         (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE | FOW_BIT_W | FOW_BIT_C | FOW_BIT_E |FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)

#define NUM_FOW_ENTRIES fow_all

// straights
#define fow_EEE         (FOW_BIT_SE | FOW_BIT_E | FOW_BIT_NE)
#define fow_NNN         (FOW_BIT_NE | FOW_BIT_N | FOW_BIT_NW)
#define fow_WWW         (FOW_BIT_NW | FOW_BIT_W | FOW_BIT_SW)
#define fow_SSS         (FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)

// corners
#define fow_CNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N |FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CNW         (FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W |FOW_BIT_SW | FOW_BIT_C | FOW_BIT_NE)
#define fow_CSW         (FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S |FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CSE         (FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E |FOW_BIT_NE | FOW_BIT_C | FOW_BIT_SW)

	// joins
#define fow_JNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
#define fow_JNW         (FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W)
#define fow_JSW         (FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S)
#define fow_JSE         (FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E)

// max num of sprite rects
#define MAX_FOW_GRAPHICS 14

struct SDL_Texture;

enum class FOGTYPE : uint
{
	SHROUD,
	FOG,
	VISIBLE,
	MAX
};

struct FOWTILE
{
	unsigned short m_bits;
	unsigned short m_bits2;
};

// we have to add this emitter to any entity what we want
// fow emitter updates auto its visibility to fog data map
class FowEmitter
{
public:
	FowEmitter();
	FowEmitter(uint radius);
	~FowEmitter();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	void SetPos(iPoint pos);

private:

public:
	bool to_delete = false;


private:
	
	uint radius;
	iPoint position; // on TILE values
	//iPoint previousPosition; // on TILE values
};


class j1FowManager : public j1Module
{
public:
	j1FowManager();
	~j1FowManager();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	void CreateFogDataMap(const uint width, const uint height);
	FowEmitter* AddFogEmitter(uint radius);
	bool IsThisTileVisible(iPoint position) const;


//private:
public:
	FOWTILE* GetFogTileAt(iPoint position) const;
	bool CheckFogMapBoundaries(iPoint position) const;

private:
	SDL_Rect foggyTilesRects[MAX_FOW_GRAPHICS];
	SDL_Texture* smoothFogTex = nullptr;
	signed char fog_rects_table[NUM_FOW_ENTRIES];
	bool debug = false;
	// stores data map size ---
	uint width;
	uint height;
	// ------------------------
	FOWTILE* fogDataMap = nullptr; // stores entire tilemap states for every tile
	std::list<FowEmitter*> currentEmitters; // stores all emitters on entities

	friend class FowEmitter;

};


#endif // !__J1FOWMANAGER_H__
