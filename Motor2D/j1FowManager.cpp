#include "j1App.h"
#include "j1FowManager.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"

j1FowManager::j1FowManager()
{
	name.assign("Fog of War Manager");
}

j1FowManager::~j1FowManager() {}

bool j1FowManager::Awake(pugi::xml_node& node)
{
	bool ret = true;

	return ret;
}

bool j1FowManager::Start()
{
	bool ret = true;

	debugPropagationTex = App->tex->LoadTexture("maps/meta2.png");

	// change hint render scale quality to linear for this texture
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // linear
	uint w, h;
	App->win->GetWindowSize(w, h);
	blurredFogTex = App->tex->CreateTargetTexture(w, h);
	swapTexForBlur = App->tex->CreateTargetTexture(w / 4, h / 4);
	// reset hint change
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // nearest neighbour

	//debug = true;

	return ret;
}

bool j1FowManager::PreUpdate()
{
	bool ret = true;

	return ret;
}

void j1FowManager::PrintFrontiersToTex(std::queue<iPoint>& frontier)
{
	std::queue<iPoint> tempFrontier = frontier;
	
	if (!tempFrontier.empty())
	{
		// change render to texture
		SDL_SetRenderTarget(App->render->renderer, blurredFogTex);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 0);
		// clear last frame
		SDL_RenderClear(App->render->renderer);

		while (!tempFrontier.empty())
		{
			iPoint frontierPos = tempFrontier.front();
			tempFrontier.pop();

			iPoint drawPos = App->map->MapToWorld(frontierPos.x, frontierPos.y);
			SDL_Rect rect = { 452,233,64,64 };
			SDL_Rect dstRect = { drawPos.x,drawPos.y, rect.w, rect.h };
			//App->render->Blit(debugPropagationTex, drawPos.x, drawPos.y, &rect);
			//SDL_RenderCopyEx(App->render->renderer, blurredFogTex, &sprite_rect, &section_to_print, 0, 0, SDL_FLIP_NONE);


			SDL_RenderCopy(App->render->renderer, App->entityFactory->entities_atlas_tex, &rect, &dstRect);
		}

		SDL_SetRenderTarget(App->render->renderer, NULL);
	}


}

bool j1FowManager::Update(float dt)
{
	bool ret = true;

	for (std::list<FowEmitter*>::iterator iter = currentEmitters.begin(); iter != currentEmitters.end();)
	{
		if (!(*iter)->to_delete)
		{
			(*iter)->Update(dt);
			//LOG("emitter pos:%i,%i", (*iter)->position.x, (*iter)->position.y);
			//(*iter)->frontier
			PrintFrontiersToTex((*iter)->frontier);
			++iter;
		}
		else
		{
			(*iter)->CleanUp();
			delete (*iter);
			iter = currentEmitters.erase(iter);
		}
	}

	// render to texture
	// changes render to target texture

	SDL_SetRenderTarget(App->render->renderer, blurredFogTex); // this
	///*SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 0);
	//SDL_RenderClear(App->render->renderer);*/
	////SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_ADD);
	////SDL_SetTextureBlendMode(blurredFogTex, SDL_BLENDMODE_BLEND);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (fogDataMap[y * width + x] == FOGTYPE::FOG)
			{
				iPoint drawPos = App->map->MapToWorld(x, y);
				SDL_Rect rect = { 452,282,64,64 }; // 452,282 // 64,0,64,64
				SDL_Rect dstRect = { drawPos.x,drawPos.y, rect.w, rect.h };
				//App->render->Blit(debugPropagationTex, drawPos.x, drawPos.y, &rect);
				//SDL_RenderCopyEx(App->render->renderer, blurredFogTex, &sprite_rect, &section_to_print, 0, 0, SDL_FLIP_NONE);
				
				
				SDL_RenderCopy(App->render->renderer, App->entityFactory->entities_atlas_tex, &rect, &dstRect);
			}
		}
	}

	/*uint w, h;
	App->win->GetWindowSize(w, h);*/

	/*SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_Texture* swapTexForBlur = App->tex->CreateTargetTexture(w / 4, h / 4);*/
	int passes = 8;
	for (int i = 0; i < passes; ++i)
	{
		//SDL_RenderClear(App->render->renderer);
		SDL_SetRenderTarget(App->render->renderer, swapTexForBlur);
		SDL_RenderClear(App->render->renderer);
		SDL_RenderCopy(App->render->renderer, blurredFogTex, NULL, NULL);
		
		//SDL_RenderClear(App->render->renderer);
		SDL_SetRenderTarget(App->render->renderer, blurredFogTex);
		SDL_RenderClear(App->render->renderer);
		SDL_RenderCopy(App->render->renderer, swapTexForBlur, NULL, NULL);

		//SDL_RenderClear(App->render->renderer);
		
	}
	////SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_SetRenderTarget(App->render->renderer, NULL);
	//SDL_DestroyTexture(swapTexForBlur);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");


	// blurr texture
	/*for (int i = 0; i < 4; ++i)
	{*/
	/*int w, h;
	SDL_QueryTexture(blurredFogTex, NULL, NULL,&w, &h);
	SDL_Rect dstRect = { 0,0, w /2, h /2 };
	SDL_RenderCopy(App->render->renderer, blurredFogTex, NULL, &dstRect);*/
	/*dstRect = { 0,0, w, h };
	SDL_RenderCopy(App->render->renderer, blurredFogTex, NULL, &dstRect);*/
	//}

	//Reset render target 
	//SDL_SetRenderTarget(App->render->renderer, NULL);

	/*int w, h;
	SDL_QueryTexture(blurredFogTex, NULL, NULL, &w, &h);
	SDL_Rect dstRect = { 0,0, w / 2, h / 2 };
	SDL_RenderCopy(App->render->renderer, blurredFogTex, NULL, &dstRect);*/

	return ret;
}

bool j1FowManager::PostUpdate()
{
	bool ret = true;

	// prints debug visibility zone
	/*if (debug)
	{
		for (std::list<FowEmitter*>::iterator iter = currentEmitters.begin(); iter != currentEmitters.end(); ++iter)
		{
			(*iter)->PostUpdate();
		}
	}*/

	SDL_SetTextureAlphaMod(blurredFogTex, 225);
	App->render->Blit(blurredFogTex, 0, 0);//-App->render->camera.x, -App->render->camera.y);
	// for every fogged tile, print the fog
	// only for tiles on screen margins and on the fog list
	//TODO
	/*for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (fogDataMap[y * width + x] == FOGTYPE::FOG)
			{
				iPoint drawPos = App->map->MapToWorld(x, y);
				SDL_Rect rect = { 64,0,64,64 };
				App->render->Blit(debugPropagationTex, drawPos.x, drawPos.y, &rect);
			}
		}
	}
*/
	return ret;
}

bool j1FowManager::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1FowManager::CreateFogDataMap(uint width, uint height)
{
	this->width = width;
	this->height = height;

	if (fogDataMap != nullptr)
	{
		RELEASE_ARRAY(fogDataMap);
	}

	fogDataMap = new FOGTYPE[width * height];
	memset(fogDataMap, NULL, width*height);
}

FOGTYPE j1FowManager::GetFogTileAt(iPoint position) const
{
	FOGTYPE ret = FOGTYPE::FOG; // if is not inside boundaries, shroud

	if (CheckFogMapBoundaries(position))
	{
		ret = fogDataMap[(position.y * width) + position.x];
	}
	/*else
		ret = FOGTYPE::SHROUD;*/

	return ret;
}

bool j1FowManager::CheckFogMapBoundaries(iPoint position) const
{
	return (position.x >= 0 && position.x < width &&
		position.y >= 0 && position.y < height);
}

FowEmitter* j1FowManager::AddFogEmitter(uint radius)
{
	FowEmitter* ret = nullptr;

	ret = new FowEmitter(radius);

	if (ret != nullptr)
		currentEmitters.push_back(ret);

	return ret;
}

void j1FowManager::SetFogTypeToTile(FOGTYPE type, iPoint position)
{
	if (CheckFogMapBoundaries(position))
	{
		fogDataMap[position.y * width + position.x] = type;
	}
}


// FOG OF WAR EMITTER CLASS --------------------------------------------------------------

FowEmitter::FowEmitter(){}

FowEmitter::FowEmitter(uint radius) : radius(radius)
{
	
}

FowEmitter::~FowEmitter(){}

bool FowEmitter::Start()
{
	bool ret = true;

	return ret;
}

bool FowEmitter::Update(float dt)
{
	bool ret = true;

	// if we change from tile, recalc visibility for this emitter
	if (previousPosition != position)
	{
		// updates last visibility positions to fog state
		RemoveLastVisibilitySpot();
		// put new position on frontier
		frontier.push(position);
		visited.push_back(position);
		// propagate new visibility positions
		PropagateBFS();
		// filter
		FilterLastVisibles();
		// updates data map
		UpdateVisibilitySpot();
		// updates previous position to this new position
		previousPosition = position;
	}

	return ret;
}

bool FowEmitter::PostUpdate()
{
	bool ret = true;

	for (std::list<iPoint>::iterator lastVisitedPos = visited.begin(); lastVisitedPos != visited.end(); ++lastVisitedPos)
	{
		iPoint drawPos = App->map->MapToWorld((*lastVisitedPos).x, (*lastVisitedPos).y);
		SDL_Rect rect = { 0,0,64,64 };
		App->render->Blit(App->fogOfWar->debugPropagationTex, drawPos.x, drawPos.y, &rect);
	}

	return ret;
}

bool FowEmitter::CleanUp()
{
	bool ret = true;

	return ret;
}

void FowEmitter::SetPos(iPoint position)
{
	this->position = App->map->WorldToMap(position.x, position.y);
}

// propagates only when entity changes tile position
bool FowEmitter::PropagateBFS()
{
	bool ret = true;

	// propagate steps based on radius
	if (!frontier.empty())
	{
		for (uint currentPropStep = 1; currentPropStep < radius; ++currentPropStep)
		{

			int steps = frontier.size();
			for (int i = 0; i < steps; ++i)
			{
				//LOG("DOING: %i", i);
				iPoint currentTile = frontier.front();
				frontier.pop(); // pops last queue value

				// each relative subtile neighbour
				iPoint neighbours[4];
				neighbours[0] = { currentTile.x, currentTile.y - 1 }; // N
				neighbours[1] = { currentTile.x + 1, currentTile.y }; // E
				neighbours[2] = { currentTile.x, currentTile.y + 1 }; // S
				neighbours[3] = { currentTile.x - 1, currentTile.y }; // W

				for (int i = 0; i < 4; ++i)
				{
					if (std::find(visited.begin(), visited.end(), neighbours[i]) != visited.end())
						continue;
					else
					{
						// if the neighbour is inside map boundaries
						if (App->fogOfWar->CheckFogMapBoundaries(neighbours[i]))
						{
							frontier.push(neighbours[i]);
							visited.push_back(neighbours[i]);
						}
					}
				}
			}
		}
	}
	
	return ret;
}

bool FowEmitter::FilterLastVisibles()
{
	// compare last propagation with the data of fog map

	for (std::list<iPoint>::iterator lastVisited = visited.begin(); lastVisited != visited.end(); )
	{
		if (App->fogOfWar->GetFogTileAt((*lastVisited)) == FOGTYPE::VISIBLE)
		{
			lastVisited = visited.erase(lastVisited);
		}
		else
			++lastVisited;
	}

	return true;
}

bool FowEmitter::UpdateVisibilitySpot()
{
	bool ret = true;

	// updates last updated vector positions to fog data map
	// only turns "on" visibility, means the player has sight on this zone
	for (std::list<iPoint>::iterator position = visited.begin(); position != visited.end(); ++position)
	{
		if (App->fogOfWar->GetFogTileAt((*position)) != FOGTYPE::VISIBLE)
			App->fogOfWar->SetFogTypeToTile(FOGTYPE::VISIBLE, (*position));
	}
	
	return ret;
}

bool FowEmitter::RemoveLastVisibilitySpot()
{
	bool ret = true;

	for (std::list<iPoint>::iterator lastVisited = visited.begin(); lastVisited != visited.end(); ++lastVisited)
	{
		if (App->fogOfWar->GetFogTileAt((*lastVisited)) == FOGTYPE::VISIBLE)
				App->fogOfWar->SetFogTypeToTile(FOGTYPE::FOG, (*lastVisited));
	}
	
	// "clear" frontier queue ---
	std::queue<iPoint> empty;
	std::swap(frontier, empty);
	// --------------------------
	visited.clear();

	return ret;
}