#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GUIText;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//void CameraLogic(float dt);

	//bool SearchValidCameraPos();

	/*bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;*/

private:
	//void DebugInput();
	// debug path generation with mouse

public:

private:
	
	fPoint cameraPos = { 0,0 };
	
};

#endif // __j1SCENE_H__