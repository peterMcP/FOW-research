#ifndef _PLAYER_H__
#define _PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();

	bool Update(float dt);

public:

	void SimpleInput();
		
};
#endif // !_PLAYER_H__
