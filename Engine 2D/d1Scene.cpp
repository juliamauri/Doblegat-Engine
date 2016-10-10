#include "c2Defs.h"
#include "c2Log.h"
#include "d1App.h"
#include "d1Input.h"
#include "d1Textures.h"
#include "d1Audio.h"
#include "d1Render.h"
#include "d1Window.h"
#include "d1Map.h"
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
	App->map->Load("iso.tmx");
	
	return true;
}

// Called each loop iteration
bool d1Scene::PreUpdate()
{
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
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	App->map->Draw();
	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	c2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);
	App->win->SetTitle(title.GetString());
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
