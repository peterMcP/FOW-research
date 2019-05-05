#include "j1App.h"
#include "j1EntityFactory.h"
#include "player.h"
#include "Enemy.h"
#include "Ward.h"
#include "j1Textures.h"
#include "j1Map.h"

j1EntityFactory::j1EntityFactory()
{
	name.assign("SimpleEntityFactory");
}

j1EntityFactory::~j1EntityFactory()
{}

bool j1EntityFactory::Awake(pugi::xml_node& node)
{
	bool ret = true;


	return ret;
}

bool j1EntityFactory::Start()
{
	bool ret = true;

	entities_atlas_tex = App->tex->LoadTexture("textures/entities_spritesheet.png");

	return ret;
}

bool j1EntityFactory::PreUpdate()
{
	bool ret = true;

	// check if we have any entity to delete
	for (std::list<Entity*>::iterator iter = entities.begin(); iter != entities.end(); )
	{
		if ((*iter)->to_delete)
		{
			delete (*iter);
			iter = entities.erase(iter);
		}
		else
			++iter;
	}

	return ret;
}

bool j1EntityFactory::Update(float dt)
{
	bool ret = true;

	for (std::list<Entity*>::iterator iter = entities.begin(); iter != entities.end(); ++iter)
	{
		(*iter)->Update(dt);
	}

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	bool ret = true;

	for (std::list<Entity*>::iterator iter = entities.begin(); iter != entities.end(); ++iter)
	{
		// TODO4: check entity types of our interest (ENEMY) to draw or not outside visibility zone
		
		
		(*iter)->Draw();
	}

	return ret;
}

bool j1EntityFactory::CleanUp()
{
	bool ret = true;

	// unload possible entities ----
	std::list<Entity*>::iterator iter = entities.begin();

	for (; iter != entities.end();)
	{
		(*iter)->CleanUp();
		iter = entities.erase(iter);
	}
	// -----------------------------
	
	// unload textures -------------

	if (entities_atlas_tex != nullptr)
		if (!App->tex->UnloadTexture(entities_atlas_tex))
			ret = false;
	// -----------------------------

	return ret;
}

Entity* j1EntityFactory::AddEntity(EntityType type, iPoint position)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:
	{
		ret = new Player(position);
		ret->spriteRect = { 0,76,71,70 };
		break;
	}
	case EntityType::ENEMY:
	{
		ret = new Enemy();
		ret->spriteRect = { 0,0,74,66 };
		break;
	}
	case EntityType::WARD:
	{
		ret = new Ward(position);
		ret->spriteRect = { 101, 83, 22,63 };
		break;
	}
	
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->position = position;
		ret->type = type;
		entities.push_back(ret);
	}

	return ret;
}