#include "j1App.h"
#include "Entity.h"
#include "j1EntityFactory.h"
#include "j1Render.h"

Entity::Entity()
{

}

Entity::~Entity() {}

bool Entity::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Entity::CleanUp()
{
	bool ret = true;

	return ret;
}

bool Entity::Draw()
{
	bool ret = true;

	if (App->entityFactory->entities_atlas_tex != nullptr)
		App->render->Blit(App->entityFactory->entities_atlas_tex, position.x, position.y, &spriteRect);

	return ret;
}