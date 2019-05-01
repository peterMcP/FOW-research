#ifndef __J1FOWMANAGER_H__
#define __J1FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include <list>
#include <vector>
#include <queue>

struct SDL_Texture;

enum class FOGTYPE : uint
{
	SHROUD,
	FOG,
	VISIBLE,
	MAX
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
	bool PropagateBFS();
	bool RemoveLastVisibilitySpot();
	// updates data map
	bool UpdateVisibilitySpot();
	bool FilterLastVisibles();

public:
	bool to_delete = false;
	std::queue<iPoint> frontier;


private:
	uint radius;
	iPoint position; // on TILE values
	iPoint previousPosition; // on TILE values

	// stores all positions from last emission
	//std::vector<iPoint> lastVisibilityPositions;
	// bfs relatives
	//std::queue<iPoint> frontier;
	std::list<iPoint> visited;
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
	//bool IsTileShroud(int x, int y) const;
	void PrintFrontiersToTex(std::queue<iPoint>& frontier);

protected:
	void SetFogTypeToTile(FOGTYPE type, iPoint position);

//private:
public:
	FOGTYPE GetFogTileAt(iPoint position) const;
	bool CheckFogMapBoundaries(iPoint position) const;

private:
	// DEBUG
	SDL_Texture* debugPropagationTex = nullptr;
	bool debug = false;
	//
	SDL_Texture* blurredFogTex = nullptr;
	SDL_Texture* swapTexForBlur = nullptr;
	// stores data map size ---
	uint width;
	uint height;
	// ------------------------
	FOGTYPE* fogDataMap = nullptr; // stores entire tilemap states for every tile
	std::list<FowEmitter*> currentEmitters; // stores all emitters on entities

	friend class FowEmitter;

};




#endif // !__J1FOWMANAGER_H__
