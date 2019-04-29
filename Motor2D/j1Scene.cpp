#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Window.h"

#include "Brofiler/Brofiler.h"




j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	bool ret = true;

	LOG("Loading Scene");

	/*uint width, height;
	App->win->GetWindowSize(width, height);*/

	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("maps/level.tmx");
		
	//// create walkability map
	//if (App->map->map_loaded)
	//{
	//	int w, h;
	//	uchar* data = NULL;
	//	if (App->map->CreateWalkabilityMap(w, h, &data))
	//		App->pathfinding->SetMap(w, h, data);

	//	RELEASE_ARRAY(data);
	//}

	//debug_tex = App->tex->LoadTexture("maps/calculatedPathTex.png");



	//SearchValidCameraPos();
	
	// adds player
	App->entityFactory->AddEntity(EntityType::PLAYER, { 100,100 });
	// add enemy
	App->entityFactory->AddEntity(EntityType::ENEMY, { 200,200 });
	// add allied building
	App->entityFactory->AddEntity(EntityType::ALLIED_BUILDING, { 400,400 });
	// add enemies building
	App->entityFactory->AddEntity(EntityType::ENEMY_BUILDING, { 0,0 });

	App->render->camera.x = 200;

	
	return true;
}

bool j1Scene::PreUpdate() {
	

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		App->map->showNavLayer = !App->map->showNavLayer;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("SCENE UPDATE", Profiler::Color::DeepSkyBlue);

	// checks for debug input
	//DebugInput();
	// ----------------------

	


	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	
	return true;
}

//void j1Scene::CameraLogic(float dt)
//{
//	iPoint offset = {0, 0};
//	//-- The screen is horizontally divided into 8 parts (see config.xml)
//	if (App->object->player->flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL) {
//		//-- Place the player on the 5th part
//		offset.x = horizontalScreenDivision * 5.0f;
//	}
//	else {
//		//-- Place the player on the 3rd part
//		offset.x = horizontalScreenDivision * 3.0f;
//	}
//	//-- The screen is vertically divided into 6 parts (see config.xml)
//	//-- Place the player on the 5th part
//	offset.y = verticalScreenDivision * 5.0f;	
//
//	iPoint playerPivotPos;
//	playerPivotPos.x = -(int)(App->object->player->position.x * (int)App->win->GetScale()); // center of current player pivot
//	playerPivotPos.y = -(int)(App->object->player->position.y * (int)App->win->GetScale());
//
//	fPoint target;
//	target.x = (playerPivotPos.x + (int)offset.x);
//	target.y = (playerPivotPos.y + (int)offset.y);
//
//	cameraPos.x += (target.x - App->render->camera.x) * cameraHMultiplier * dt;
//	if (App->render->camera.y >= target.y) {
//		cameraPos.y += (target.y - App->render->camera.y) * cameraJumpMultiplier * dt;
//	}
//	else {
//		cameraPos.y += (target.y - App->render->camera.y) * cameraFallMultiplier  *dt;
//	}
//
//	App->render->camera.x = cameraPos.x;
//	App->render->camera.y = cameraPos.y;
//	
//}



 //bool j1Scene::SearchValidCameraPos()
 //{
	// // update offset
	// uint width, height = 0;
	// App->win->GetWindowSize(width, height);

	// float x = width * 0.25f * 1.5f; // situates player on the middle of second screen partition(of 4)
	// float y = height * 0.33f *2.5f; // 

	// iPoint offset = { (int)x , (int)y };
	// // -------------
	// fPoint startPosition;
	// startPosition.x = App->map->playerData.x;
	// startPosition.y = App->map->playerData.y;

	// App->render->camera.x = cameraPos.x = (-startPosition.x * App->win->GetScale()) + offset.x;
	// App->render->camera.y = cameraPos.y = (-startPosition.y * App->win->GetScale()) + offset.y;


	// return true;
 //}
