#include "j1App.h"
#include "Ward.h"

Ward::Ward(iPoint position) : Entity(type, position)
{
	// adds visibility emitter
	fogVisibilityEmitter = App->fogOfWar->AddFogEmitter(4);
	fogVisibilityEmitter->SetPos(position);
	// define max life in ms
	wardLifeMs = 10000;
	// start life timer
	wardLifeTimer.Start();
}

Ward::~Ward() 
{
	fogVisibilityEmitter->to_delete = true;
}

bool Ward::Update(float dt)
{
	bool ret = true;

	if (wardLifeTimer.Read() >= wardLifeMs)
		to_delete = true;

	return ret;
}

bool Ward::CleanUp()
{
	bool ret = true;

	return ret;
}