#include "c2Defs.h"
#include "c2Log.h"

#include "d1App.h"
#include "d1Input.h"
#include "d1Textures.h"
#include "d1Audio.h"
#include "d1Render.h"
#include "d1Window.h"
#include "d1Map.h"
#include "d1PathFinding.h"
#include "d1UIManager.h"
#include "d1Scene.h"


d1Scene::d1Scene() : d1Module()
{
	name.create("scene");
}

// Destructor
d1Scene::~d1Scene()
{}

// Called before render is available
bool d1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool d1Scene::Start()
{
	if (App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	//Definition UIElements
	uint width = 0;
	uint height = 0;
	App->win->GetWindowSize(width,height);

	
	banner = (UIImage*)App->uimanager->addUIComponent(UIComponent_TYPE::UIIMAGE);
	banner->Set({ (int)width / 2, (int)height / 4, 328, 103 }, { 485, 829, 328, 103 });

	text = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
	text->Set(width / 2, height / 4 - 120, "Hello World");

	check_test = (UICheckbutton*)App->uimanager->addUIComponent(UIComponent_TYPE::UICHECKBUTTON);
	check_test->Set({ 100, 100, 328, 103 }, { 485, 829, 328, 103 }, { 100, 100, 328, 103 });
	check_test->title->Set(150, 75,"Test Chech Button");

	select_test = (UISelectOption*)App->uimanager->addUIComponent(UIComponent_TYPE::UISELECTOPTION);
	select_test->Set({ 100,500,100,50 }, { 485, 829, 100, 50 });
	select_test->title->Set(100, 475, "Select Your Destiny");
	select_test->AddOption("OMG");
	select_test->AddOption("YOU'RE FAGGOT");
	select_test->AddOption("LET ME SUICIDE");
	select_test->AddOption("FUCK OFF");

	return true;
}

// Called each loop iteration
bool d1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool d1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);;

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	c2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const c2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	
	//UISelection
	if (text->stat == UIComponent_Stat::SELECTED)
	{
		if(right_click == true)
			text->ChangeText("Hello World - right click");
		else if(left_click == true)
			text->ChangeText("Hello World - left click");
		else
			text->ChangeText("It's hover");
	} 
	else if (text->stat == UIComponent_Stat::UNSELECTED)
	{
		text->ChangeText("Hello World");
		right_click = false;
		left_click = false;
	}
	else if (text->stat == UIComponent_Stat::CLICKR_DOWN)
	{
		right_click = true;
		left_click = false;
	}
	else if (text->stat == UIComponent_Stat::CLICKL_DOWN)
	{
		left_click = true;
		right_click = false;
	}
	

	return true;
}

// Called each loop iteration
bool d1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool d1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
