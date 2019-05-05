#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Brofiler/Brofiler.h"

j1Render::j1Render() : j1Module()
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 255;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		App->vsync = true;
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}
	else {
		App->vsync = false;
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	CameraDebug(dt);
	return true;
}

void j1Render::CameraDebug(float dt)
{
	uint screenW, screenH = 0;
	App->win->GetWindowSize(screenW, screenH);

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT) {
		cameraDebug = true;
		camera.x -= CAMERA_MOVE_SPEED * dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT) {
		cameraDebug = true;
		camera.x += CAMERA_MOVE_SPEED * dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT) {
		cameraDebug = true;
		camera.y += CAMERA_MOVE_SPEED * dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT) {
		cameraDebug = true;
		camera.y -= CAMERA_MOVE_SPEED * dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN && zoomedOutSize < MAX_ZOOM)
	{
		cameraDebug = true;
		zoomedOutSize++;
		SDL_RenderSetLogicalSize(renderer, screenW * zoomedOutSize, screenH * zoomedOutSize);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_9) == KEY_DOWN && zoomedOutSize > 1)
	{
		cameraDebug = true;
		zoomedOutSize--;
		SDL_RenderSetLogicalSize(renderer, screenW * zoomedOutSize, screenH * zoomedOutSize);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_REPEAT) {
		cameraDebug = false;
		zoomedOutSize = 1;
		SDL_RenderSetLogicalSize(renderer, screenW * zoomedOutSize, screenH * zoomedOutSize);
	}

	//if (App->input->GetKey(SDL_SCANCODE_KP_MULTIPLY) == KEY_DOWN)
	//{
	//	if (showTilemapGrid == true) { showTilemapGrid = false; }
	//	else { showTilemapGrid = true; }
	//}
}


bool j1Render::PostUpdate()
{
	BROFILER_CATEGORY("Renderer PostUpdate", Profiler::Color::Gold);

	//Draw the borders of the screen

	SDL_Color color = { 255, 255, 255, 255 };

	int borderWidth = DEFAULT_BORDER_WIDTH * zoomedOutSize;
	uint uScreenWidth, uScreenHeight;
	int screenWidth, screenHeight;
	App->win->GetWindowSize(uScreenWidth, uScreenHeight);
	screenWidth = (int)uScreenWidth;
	screenHeight = (int)uScreenHeight;

	SDL_Rect rectUp = { 0, -borderWidth, screenWidth, borderWidth };
	DrawQuad(rectUp, color.r, color.g, color.b, color.a, true, false);
	SDL_Rect rectDown = { 0, screenHeight, screenWidth, borderWidth };
	DrawQuad(rectDown, color.r, color.g, color.b, color.a, true, false);
	SDL_Rect rectLeft = { -(int)borderWidth, 0, borderWidth, screenHeight };
	DrawQuad(rectLeft, color.r, color.g, color.b, color.a, true, false);
	SDL_Rect rectRight = { screenWidth, 0, borderWidth, screenHeight };
	DrawQuad(rectRight, color.r, color.g, color.b, color.a, true, false);

	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, SDL_RendererFlip flip, double angle, int pivot_x, int pivot_y) const
{
	BROFILER_CATEGORY("Render Blit", Profiler::Color::Azure);

	bool ret = true;
	int scale = (int)App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;
	
	//Don't blit if the sprite is out of the screen
	uint width, height = 0;
	App->win->GetWindowSize(width, height);
	if (rect.x + rect.w < 0 || rect.y + rect.h < 0 || rect.x >(int)width || rect.y >(int)height) {
		return false;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::BlitGUI(SDL_Texture* texture, int x, int y, const SDL_Rect* section, int scale) const
{
	BROFILER_CATEGORY("Render BlitGUI", Profiler::Color::Azure);

	bool ret = true;

	SDL_Rect rect;
	rect.x = x; //* scale; // CAREFULL with this
	rect.y = y; //* scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}