#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
//#include "j1FowManager.h"
#include "j1FowManager.h"

enum class EntityType
{
	PLAYER,
	ENEMY,
	WARD
};

class Entity
{
public:
	Entity(EntityType type, iPoint position);
	virtual ~Entity();

	virtual bool Update(float dt);
	virtual	bool CleanUp();

public:
	bool Draw();

public:
	bool to_delete = false;
	EntityType type;
	iPoint position;
	SDL_Rect spriteRect;

	// add fog of war emitter
	FowEmitter* fogVisibilityEmitter = nullptr;

};

#endif // !__ENTITY_H__
