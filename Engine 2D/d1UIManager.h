#ifndef __d1GUIMANAGER_H__
#define __d1GUIMANAGER_H__

#include "c2Graph.h"

#include "d1Module.h"


struct _TTF_Font;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

enum UIComponent_Stat
{
	SELECTED,
	UNSELECTED,

	CLICKL_DOWN,
	CLICKR_DOWN,

	CLICKL_REPEAT,
	CLICKR_REPEAT,

	CLICKL_UP,
	CLICKR_UP
};

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

	UIComponent_Stat stat = UIComponent_Stat::UNSELECTED;

public:
	UIComponents(UIComponent_TYPE type) : type(type) {}

	virtual void Set() {};
};

class UILabel : public UIComponents
{
public:
	c2SString text;
	SDL_Texture* text_img;
	_TTF_Font*  font;

public:
	UILabel(UIComponent_TYPE type) : UIComponents(type) {}

	void Set(int pos_x, int pos_y, const char* text, _TTF_Font*  font = nullptr);

	void ChangeText(const char* text);
};

class UIButton : public UIComponents
{
public:
	bool clicked;
	UILabel* title;

public:
	UIButton(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);
};

class UIInput : public UIComponents
{
public:
	c2SString input_text;
	UILabel* title;

public:
	UIInput(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	const char* GetStr();
};

class UIImage : public UIComponents
{
public:
	UIImage(UIComponent_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);
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

	UIComponents* addUIComponent(UIComponent_TYPE type);

	const SDL_Texture* GetAtlas() const;

	const c2List_item<UIComponents*>* GetFirstComponent() const;

private:
	c2Graph<UIComponents*> components;

	//c2List<UIComponents*> components;

	SDL_Texture* atlas;
	c2SString atlas_file_name;



private:
	void drawAllComponents();
};
#endif // __d1GUIMANAGER_H__
