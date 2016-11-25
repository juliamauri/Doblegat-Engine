#ifndef __d1SCENE_H__
#define __d1SCENE_H__

#include "d1Module.h"

struct SDL_Texture;
class UIImage;
class UILabel;

class d1Scene : public d1Module
{
public:

	d1Scene();

	// Destructor
	virtual ~d1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* debug_tex;
	UIImage* banner;
	UILabel* text;
};

#endif // __d1SCENE_H__