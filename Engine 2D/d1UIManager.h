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
	UIComponents(UIComponent_TYPE type) : type(type) {}

	virtual void Set();

private:
	void SetImage(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void SetImage(SDL_Rect position, SDL_Rect atlas);
};

class UILabel : public UIComponents
{
public:
	c2SString text;
	SDL_Texture* text_img;
	_TTF_Font*  font;

public:
	UILabel() : UIComponents(UIComponent_TYPE::UILABEL) {}

	void Set(int pos_x, int pos_y, const char* text, _TTF_Font*  font = nullptr);

	void ChangeText(const char* text);
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

typedef UIComponents UIImage;

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

	const UIComponents* GetSelected();

	void CheckMouseInUIComponent();

private:
	c2List<UIComponents*> components;

	UIComponents* mouse_selection = NULL;

	SDL_Texture* atlas;
	c2SString atlas_file_name;

private:
	void drawAllComponents();
};
#endif // __d1GUIMANAGER_H__
