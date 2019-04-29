//#include <iostream> 
//#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1EntityFactory.h"
#include "j1Map.h"
#include "j1App.h"
#include "Brofiler/Brofiler.h"
//#include "SDL/include/SDL_thread.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);
	
	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	scene = new j1Scene();
	map = new j1Map();
	entityFactory = new j1EntityFactory();
	

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);//Input is the first module to update beacause other modules use the input during their update
	AddModule(win);
	AddModule(tex);
	AddModule(map);
	AddModule(scene);
	AddModule(entityFactory);
	AddModule(render);// render last to swap buffer

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	std::list<j1Module*>::iterator item = modules.end();

	while(item != modules.end())
	{
		RELEASE((*item));
		item = modules.erase(item);
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	bool ret = false;

	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());
		capFrames = app_config.attribute("cap_frames").as_bool();
		framerateCap = app_config.attribute("framerate_cap").as_float();
		capTime = 1000 / app_config.attribute("framerate_cap").as_int();
	}

	if(ret == true)
	{
		std::list<j1Module*>::iterator item = modules.begin();

		while(item != modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->name.data()));
		
			++item;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<j1Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("App updates", Profiler::Color::LawnGreen);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	if (ret == true)
		ret = !requestExit;

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL) {
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}
	else {
		ret = config_file.child("config");
	}

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	if (pause) {
		dt = 0.0f;
	}
	else 
	{
		dt = frame_time.ReadSec();
	}

	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("App FinishUpdate", Profiler::Color::Cyan);

	//Framerate
	//- Calculations
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}
	seconds_since_startup = startup_time.ReadSec();
	float avg_fps = float(frame_count) / seconds_since_startup;
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		capFrames = !capFrames;
	}

	static char title[256];
	std::string capFramesString;
	if (capFrames) {
		capFramesString = "ON";
	}
	else {
		capFramesString = "OFF";
	}
	std::string vsyncString;
	if (vsync) {
		vsyncString = "ON";
	}
	else {
		vsyncString = "OFF";
	}

	sprintf_s(title, 256, "FOW_Research || Last sec frames: %i | Av.FPS: %.2f | Last frame ms: %02u | Framerate cap: %s | Vsync: %s",
		frames_on_last_update, avg_fps, last_frame_ms, capFramesString.data(), vsyncString.data());
	App->win->SetTitle(title);

	//- Cap the framerate
	if (capFrames) {
		uint32 delay = MAX(0, (int)capTime - (int)last_frame_ms);
		//LOG("Should wait: %i", delay);
		//j1PerfTimer delayTimer;
		SDL_Delay(delay);
		//LOG("Has waited:  %f", delayTimer.ReadMs());
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("App PreUpdate", Profiler::Color::Turquoise);
	bool ret = true;
	j1Module* pModule = NULL;

	for(std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("App Updates", Profiler::Color::LightGreen);
	bool ret = true;
	j1Module* pModule = NULL;

	for(std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("App PostUpdates", Profiler::Color::MediumOrchid);
	bool ret = true;
	j1Module* pModule = NULL;

	for(std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<j1Module*>::reverse_iterator item = modules.rend();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}

	modules.clear();

	PERF_PEEK(ptimer);

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}


float j1App::GetDt()
{
	return dt;
}

float j1App::GetGameTime()
{
	return seconds_since_startup;
}
