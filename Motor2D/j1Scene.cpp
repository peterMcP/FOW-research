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
#include "j1FowManager.h"

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

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("maps/level.tmx");

	// creates fog of war data

	App->fogOfWar->CreateFogDataMap(App->map->data.columns, App->map->data.rows);
		
	
	// add player
	App->entityFactory->AddEntity(EntityType::PLAYER, { -160,300 });
	// add enemies
	App->entityFactory->AddEntity(EntityType::ENEMY, { 200,200 });
	

	App->render->camera.x = 400;

	
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

	int camSpeed = 5;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += camSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= camSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= camSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += camSpeed;
	}

	// DEBUG FUNCTIONALITY
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint mousePos = App->render->ScreenToWorld(x, y);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{

		// adds player
		App->entityFactory->AddEntity(EntityType::PLAYER, { mousePos.x,mousePos.y });
	}
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{

		// adds player
		App->entityFactory->AddEntity(EntityType::WARD, { mousePos.x,mousePos.y });
	}
	

	// draw map
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