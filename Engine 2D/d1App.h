#ifndef __d1APP_H__
#define __d1APP_H__

#include "c2List.h"
#include "d1Module.h"
#include "PugiXml\src\pugixml.hpp"

// Modules

class d1Window;
class d1Input;
class d1Render;
class d1Textures;
class d1Audio;
class d1FileSystem;
class d1Scene;
class d1Map;
class d1PathFinding;

class d1App
{
public:

	// Constructor
	d1App(int argc, char* args[]);

	// Destructor
	virtual ~d1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(d1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(c2List<c2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	
	d1Window*			win;
	d1Input*			input;
	d1Render*			render;
	d1Textures*			tex;
	d1Audio*			audio;
	d1Scene*			scene;
	d1FileSystem*		fs;
	d1Map*				map;
	d1PathFinding*		pathfinding;

private:

	c2List<d1Module*>	modules;
	uint				frames;
	float				dt;
	int					argc;
	char**				args;

	c2SString			title;
	c2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	c2SString			load_game;
	mutable c2SString	save_game;
};

extern d1App* App; // No student is asking me about that ... odd :-S

#endif