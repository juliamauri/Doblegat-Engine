#ifndef __d1GUIMANAGER_H__
#define __d1GUIMANAGER_H__

#include "d1Module.h"

struct _TTF_Font;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

enum UIComponent_TYPE
{
	UILABEL,
	UIBUTTON,
	UIINPUT,
	UIIMAGE
};

class UIComponents
{
public:
	SDL_Rect rect_position;
	SDL_Rect rect_atlas;
	UIComponent_TYPE type;

public:
	UIComponents(int pos_x, int pos_y, UIComponent_TYPE type)
	{
		rect_position.x = pos_x;
		rect_position.y = pos_y;
		this->type = type;
	}

	UIComponents(int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, UIComponent_TYPE type)
	{
		rect_position.x = pos_x;
		rect_position.y = pos_y;
		rect_position.w = pos_w;
		rect_position.h = pos_h;

		rect_atlas.x = atlas_x;
		rect_atlas.y = atlas_y;
		rect_atlas.w = atlas_w;
		rect_atlas.h = atlas_h;

		this->type = type;
	}

	UIComponents(SDL_Rect position, SDL_Rect atlas, UIComponent_TYPE type = UIComponent_TYPE::UIIMAGE) : rect_position(position), rect_atlas(atlas), type(type) { }
};

class UILabel : public UIComponents
{
public:
	SDL_Texture* text_img;
	_TTF_Font*  font;

public:
	UILabel();
	UILabel(int pos_x, int pos_y, const char* text);
};

class UIButton : public UIComponents
{
public:
	bool clicked;
	UILabel* title;

public:
	UIButton(int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, int label_x, int label_y, const char* text_title);

	UIButton(SDL_Rect position, SDL_Rect atlas, int label_x, int label_y, const char* text_title);

	void action();
};

class UIInput : public UIComponents
{
public:
	c2SString input_text;
	UILabel* title;

public:
	UIInput(int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, int label_x, int label_y, const char* text_title);

	UIInput(SDL_Rect position, SDL_Rect atlas, int label_x, int label_y, const char* text_title);

	const char* GetStr();
};

// ---------------------------------------------------
class d1UIManager : public d1Module
{
public:

	d1UIManager();

	// Destructor
	virtual ~d1UIManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	UIComponents* addUIComponent(UIComponent_TYPE type, int pos_x = 0, int pos_y = 0, int pos_w = 0, int pos_h = 0, int atlas_x = 0, int atlas_y = 0, int atlas_w = 0, int atlas_h = 0, int label_x = 0, int label_y = 0, const char* text = nullptr);
	UIComponents* addUIComponent(UIComponent_TYPE type, SDL_Rect position = {0,0,0,0}, SDL_Rect atlas = { 0,0,0,0 }, int label_x = 0, int label_y = 0, const char* text = nullptr);

	const SDL_Texture* GetAtlas() const;

	const c2List_item<UIComponents*>* GetFirstComponent() const;

private:
	c2List<UIComponents*> components;

	SDL_Texture* atlas;
	c2SString atlas_file_name;

private:
	void drawAllComponents();
};
#endif // __d1GUIMANAGER_H__
