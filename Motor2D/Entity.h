#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
//#include "j1FowManager.h"
#include "j1FowManager.h"

enum class EntityType
{
	PLAYER,
	ENEMY,
	ALLIED_BUILDING,
	ENEMY_BUILDING
};

class Entity
{
public:
	Entity(EntityType type, iPoint position);
	~Entity();

	virtual bool Update(float dt);
	virtual	bool CleanUp();

public:
	bool Draw();

public:
	EntityType type;

	iPoint position;
	SDL_Rect spriteRect;

	// TODO: add fog of war emitter
	FowEmitter* fogVisibilityEmitter = nullptr;


};

#endif // !__ENTITY_H__
