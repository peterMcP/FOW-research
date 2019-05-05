#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"

#define DEFAULT_BORDER_WIDTH 10
#define MAX_ZOOM 5
//#define AXIS_LENGTH 5000
#define CAMERA_MOVE_SPEED 200

struct ColorRGB {
	ColorRGB(){}
	ColorRGB(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b) {}
	Uint8 r = 0u;
	Uint8 g = 0u;
	Uint8 b = 0u;
};

struct ColorRGBA {
	ColorRGBA(){}
	ColorRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {}
	Uint8 r = 0u;
	Uint8 g = 0u;
	Uint8 b = 0u;
	Uint8 a = 0u;
};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	void CameraDebug(float dt);
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;

	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool BlitGUI(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, int scale = 1) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

	bool cameraDebug = false;

private:
	//Zoom out
	//iPoint movedPosition;
	int zoomedOutSize = 1;
	//bool showTilemapGrid = false;
};

#endif // __j1RENDER_H__