#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
public:
	bool Update(float dt);
	bool CleanUp();

};
#endif // !__ENEMY_H__
