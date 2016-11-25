#include "c2Defs.h"
#include "c2Log.h"
#include "d1App.h"
#include "d1Render.h"
#include "d1Textures.h"
#include "d1Fonts.h"
#include "d1Input.h"
#include "d1UIManager.h"

d1UIManager::d1UIManager() : d1Module()
{
	name.create("uimanager");
}

// Destructor
d1UIManager::~d1UIManager()
{}

// Called before render is available
bool d1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool d1UIManager::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool d1UIManager::PreUpdate()
{
	return true;
}

// Called after all Updates
bool d1UIManager::PostUpdate()
{
	drawAllComponents();
	
	return true;
}

// Called before quitting
bool d1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

UIComponents* d1UIManager::addUIComponent(UIComponent_TYPE type, int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, int label_x, int label_y, const char* text)
{
	UIComponents* ret = nullptr;

	if (type == UIComponent_TYPE::UIBUTTON)
	{
		components.add(ret = new UIButton(pos_x, pos_y, pos_w, pos_h, atlas_x, atlas_y, atlas_w, atlas_h, label_x, label_y, text));
	}
	else if (type == UIComponent_TYPE::UIIMAGE)
	{
		components.add(ret = new UIComponents(pos_x, pos_y, pos_w, pos_h, atlas_x, atlas_y, atlas_w, atlas_h, type));
	}
	else if(type == UIComponent_TYPE::UIINPUT)
	{
		components.add(ret = new UIInput(pos_x, pos_y, pos_w, pos_h, atlas_x, atlas_y, atlas_w, atlas_h, label_x, label_y, text));
	}
	else if (type == UIComponent_TYPE::UILABEL)
	{
		components.add(ret = new UILabel(label_x, label_y, text));
	}

	return ret;
}

UIComponents* d1UIManager::addUIComponent(UIComponent_TYPE type, SDL_Rect position, SDL_Rect atlas, int label_x, int label_y, const char* text)
{
	UIComponents* ret = nullptr;

	if (type == UIComponent_TYPE::UIBUTTON)
	{
		components.add(ret = new UIButton(position,atlas, label_x, label_y, text));
	}
	else if (type == UIComponent_TYPE::UIIMAGE)
	{
		components.add(ret = new UIComponents(position, atlas, type));
	}
	else if (type == UIComponent_TYPE::UIINPUT)
	{
		components.add(ret = new UIInput(position, atlas, label_x, label_y, text));
	}

	return ret;
}

// const getter for atlas
const SDL_Texture* d1UIManager::GetAtlas() const
{
	return atlas;
}

const c2List_item<UIComponents*>* d1UIManager::GetFirstComponent() const
{
	return components.start;
}

void d1UIManager::drawAllComponents()
{
	c2List_item<UIComponents*>* item = components.start;

	UIComponent_TYPE* type = nullptr;

	float dt = App->GetDT();

	for (; item != NULL; item = item->next)
	{
		type = &item->data->type;

		if (*type == UIComponent_TYPE::UIBUTTON)
		{

		}
		else if (*type == UIComponent_TYPE::UIIMAGE)
		{
			UIComponents* uiimage = item->data;

			App->render->Blit(atlas, uiimage->rect_position.x, uiimage->rect_position.y, &uiimage->rect_atlas);
		}
		else if (*type == UIComponent_TYPE::UIINPUT)
		{

		}
		else if (*type == UIComponent_TYPE::UILABEL)
		{
			UILabel* uilabel = (UILabel*)item->data;

			App->render->Blit(uilabel->text_img, uilabel->rect_position.x, uilabel->rect_position.y);
		}
	}
}

// class uimanager ---------------------------------------------------

UIButton::UIButton(int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, int label_x, int label_y, const char * text_title) : UIComponents(pos_x, pos_y, pos_w, pos_h, atlas_x, atlas_y, atlas_w, atlas_h, UIComponent_TYPE::UIBUTTON)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL, 0, 0, 0, 0, 0, 0, 0, 0, label_x, label_y, text_title);
}

UIButton::UIButton(SDL_Rect position, SDL_Rect atlas, int label_x, int label_y, const char * text_title) : UIComponents(position, atlas, UIComponent_TYPE::UIBUTTON)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL, 0, 0, 0, 0, 0, 0, 0, 0, label_x, label_y, text_title);
}

UIInput::UIInput(int pos_x, int pos_y, int pos_w, int pos_h, int atlas_x, int atlas_y, int atlas_w, int atlas_h, int label_x, int label_y, const char * text_title) : UIComponents(pos_x, pos_y, pos_w, pos_h, atlas_x, atlas_y, atlas_w, atlas_h, UIComponent_TYPE::UIINPUT)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL, 0, 0, 0, 0, 0, 0, 0, 0, label_x, label_y, text_title);
}

UIInput::UIInput(SDL_Rect position, SDL_Rect atlas, int label_x, int label_y, const char * text_title) : UIComponents(position, atlas, UIComponent_TYPE::UIINPUT)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL, 0, 0, 0, 0, 0, 0, 0, 0, label_x, label_y, text_title);
}

UILabel::UILabel(int pos_x, int pos_y, const char * text) : UIComponents(pos_x, pos_y, UIComponent_TYPE::UILABEL)
{
	text_img = App->font->Print(text, {255,0,0,0});
}
