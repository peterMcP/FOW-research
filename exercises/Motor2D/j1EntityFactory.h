#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "p2Log.h"
#include <list>
#include "Entity.h"
#include "SDL_image/include/SDL_image.h"


class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	Entity* AddEntity(EntityType type, iPoint position);

public:
	SDL_Texture* entities_atlas_tex = nullptr;
	std::list<Entity*> entities;

};




#endif // !__J1ENTITYFACTORY_H__
