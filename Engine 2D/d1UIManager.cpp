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
	CheckMouseInUIComponent();

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

UIComponents* d1UIManager::addUIComponent(UIComponent_TYPE type)
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

// const getter for atlas
const SDL_Texture* d1UIManager::GetAtlas() const
{
	return atlas;
}

const c2List_item<UIComponents*>* d1UIManager::GetFirstComponent() const
{
	return components.start;
}

const UIComponents * d1UIManager::GetSelected()
{
	return mouse_selection;
}

void d1UIManager::CheckMouseInUIComponent()
{
	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);
	bool not_selected = true;

	if (mouse_selection != nullptr)
	{
		if ((x_mouse > mouse_selection->rect_position.x) && (x_mouse < mouse_selection->rect_position.x + mouse_selection->rect_position.w) && (y_mouse > mouse_selection->rect_position.y) && (y_mouse < mouse_selection->rect_position.y + mouse_selection->rect_position.h))
		{
			not_selected = false;
		}
		else
			not_selected = true;
	}
	else
	{
		not_selected = true;
	}

	if (not_selected)
	{
		c2List_item<UIComponents*>* item = components.start;

		for (; item != NULL; item = item->next)
		{
			UIComponents* uielement = item->data;

			if ((x_mouse > uielement->rect_position.x) && (x_mouse < uielement->rect_position.x + uielement->rect_position.w) && (y_mouse > uielement->rect_position.y) && (y_mouse < uielement->rect_position.y + uielement->rect_position.h))
			{
				mouse_selection = uielement;
				not_selected = false;
				break;
			}
			else
				mouse_selection = nullptr;
		}
	}

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

			App->render->Blit(atlas, uiimage->rect_position.x - App->render->camera.x, uiimage->rect_position.y - App->render->camera.y, &uiimage->rect_atlas);
		}
		else if (*type == UIComponent_TYPE::UIINPUT)
		{

		}
		else if (*type == UIComponent_TYPE::UILABEL)
		{
			UILabel* uilabel = (UILabel*)item->data;

			App->render->Blit(uilabel->text_img, uilabel->rect_position.x - App->render->camera.x, uilabel->rect_position.y - App->render->camera.y);
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

void UILabel::Set(int pos_x, int pos_y, const char * text, _TTF_Font*  font)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;

	text_img = App->font->Print(text, { 255,0,0,0 });

	App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);
}

void UILabel::ChangeText(const char* text)
{
	if (this->text != text)
	{
		text_img = App->font->Print(text, { 255,0,0,0 });

		App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);

		this->text = text;
	}
}

void UIComponents::SetImage(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UIComponents::SetImage(SDL_Rect position, SDL_Rect atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}
