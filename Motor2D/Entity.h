#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"

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
	Entity();
	~Entity();

	virtual bool Update(float dt);
	virtual	bool CleanUp();

public:
	bool Draw();

public:
	EntityType type;

	iPoint position;
	SDL_Rect spriteRect;


};

#endif // !__ENTITY_H__
