#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"

class Building : public Entity
{
public:
	Building(EntityType type, iPoint position);
	~Building();

public:
	bool Update(float dt);
	bool CleanUp();
};
#endif // !__BUILDING_H__
