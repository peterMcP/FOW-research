#include "j1App.h"
#include "Building.h"

Building::Building(EntityType entType, iPoint position) : Entity(entType, position)
{
	type = entType;
	
	if (type == EntityType::ALLIED_BUILDING)
	{
		fogVisibilityEmitter = App->fogOfWar->AddFogEmitter(10);
		
		// FOR TEXTURE PIVOT we must to offset this position
		iPoint offsetedPos = position;
		offsetedPos.x += 262;	//this values corresponds to pivot of the sprite rect
		offsetedPos.y += 236;

		fogVisibilityEmitter->SetPos(offsetedPos);
	}
}

Building::~Building() {}

bool Building::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Building::CleanUp()
{
	bool ret = true;

	return ret;
}