#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "j1Timer.h"

class Ward : public Entity
{
public:
	Ward(iPoint position);
	~Ward();

public:
	bool Update(float dt);
	bool CleanUp();

private:
	j1Timer wardLifeTimer;
	uint32 wardLifeMs;
};
#endif // !__BUILDING_H__
