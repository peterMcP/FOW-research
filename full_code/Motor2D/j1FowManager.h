#ifndef __J1FOWMANAGER_H__
#define __J1FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include <list>

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
#define MAX_FOW_GRAPHICS 16
// max radius precomputed masks
#define FOW_RADIUS_MIN 3
#define FOW_RADIUS_MAX 4
#define FOW_MAX_RADIUS_LENGTH ((FOW_RADIUS_MAX * 2) + 1)

struct SDL_Texture;

struct FOWTILE
{
	unsigned short m_bits_fog;
	unsigned short m_bits_shroud;
};

// we have to add this emitter to any entity what we want
// fow emitter updates auto its visibility to fog data map
class FowEmitter
{
public:
	FowEmitter();
	FowEmitter(uint radius);
	~FowEmitter();

	bool Update(float dt);
	bool CleanUp();

public:
	void SetPos(iPoint pos);

private:
	std::list<iPoint> GetTilesAffected() const;
	void ApplyMaskToMap(std::list<iPoint> tileList);

public:
	bool to_delete = false;

private:
	uint radius;
	iPoint position; // on TILE values
};


class j1FowManager : public j1Module
{
	friend class FowEmitter;

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

public:
	FOWTILE* GetFogTileAt(iPoint position) const;
	bool CheckFogMapBoundaries(iPoint position) const;

private:
	void DebugMouseDrawTilePos() const;

private:
	SDL_Rect foggyTilesRects[MAX_FOW_GRAPHICS];
	SDL_Texture* smoothFogTex = nullptr;
	SDL_Texture* debugFogTex = nullptr;
	signed char fog_rects_table[NUM_FOW_ENTRIES + 1];
	bool debug = false;
	// stores data map size ---
	uint width;
	uint height;
	// ------------------------
	FOWTILE* fogDataMap = nullptr; // stores entire tilemap states for every tile
	std::list<FowEmitter*> currentEmitters; // stores all emitters on entities

	

protected:

	// precomputed shape masks
	unsigned short area_mask[2][FOW_MAX_RADIUS_LENGTH * FOW_MAX_RADIUS_LENGTH] =
	{
		// circle radius 3
		{
				fow_all, fow_all, fow_CNW, fow_NNN, fow_CNE, fow_all, fow_all,
				fow_all, fow_CNW, fow_JNW, fow_non, fow_JNE, fow_CNE, fow_all,
				fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_JNE, fow_CNE,
				fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE,
				fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_JSE, fow_CSE,
				fow_all, fow_CSW, fow_JSW, fow_non, fow_JSE, fow_CSE, fow_all,
				fow_all, fow_all, fow_CSW, fow_SSS, fow_CSE, fow_all, fow_all,
		},
		// circle radius 4
		{
			fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,
			fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,
			fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,
			fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,
			fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
			fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,
			fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,
			fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,
			fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,
		},
		// etc
		// ...
	};

};


#endif // !__J1FOWMANAGER_H__
