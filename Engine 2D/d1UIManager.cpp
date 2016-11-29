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
	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);

	c2List_item<UIComponents*>* item = components.start;

	for (; item != NULL; item = item->next)
	{
		UIComponents* component = item->data;

		if ((x_mouse > component->rect_position.x) && (x_mouse < component->rect_position.x + component->rect_position.w) && (y_mouse > component->rect_position.y) && (y_mouse < component->rect_position.y + component->rect_position.h))
		{
			component->stat = UIComponent_Stat::SELECTED;

			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				if(component->stat == UIComponent_Stat::SELECTED)
					component->stat = UIComponent_Stat::CLICKL_DOWN;
				else
					component->stat = UIComponent_Stat::CLICKL_REPEAT;
			}
			else if (App->input->GetMouseButtonDown(1) == KEY_UP)
			{
				if (component->stat == UIComponent_Stat::CLICKL_REPEAT || component->stat == UIComponent_Stat::CLICKL_DOWN)
					component->stat = UIComponent_Stat::CLICKL_UP;
				else
					component->stat = UIComponent_Stat::SELECTED;
			}

			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
			{
				if (component->stat == UIComponent_Stat::SELECTED)
					component->stat = UIComponent_Stat::CLICKR_DOWN;
				else
					component->stat = UIComponent_Stat::CLICKR_REPEAT;
			}
			else if (App->input->GetMouseButtonDown(3) == KEY_UP)
			{
				if (component->stat == UIComponent_Stat::CLICKR_REPEAT || component->stat == UIComponent_Stat::CLICKR_DOWN)
					component->stat = UIComponent_Stat::CLICKR_UP;
				else
					component->stat = UIComponent_Stat::SELECTED;
			}

		}
		else
			component->stat = UIComponent_Stat::UNSELECTED;
	
	}
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
		components.add(ret = new UIButton(UIComponent_TYPE::UIBUTTON));
	}
	else if (type == UIComponent_TYPE::UIIMAGE)
	{
		components.add(ret = new UIComponents(UIComponent_TYPE::UIIMAGE));
	}
	else if(type == UIComponent_TYPE::UIINPUT)
	{
		components.add(ret = new UIInput(UIComponent_TYPE::UIINPUT));
	}
	else if (type == UIComponent_TYPE::UILABEL)
	{
		components.add(ret = new UILabel(UIComponent_TYPE::UILABEL));
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
			UIComponents* uibutton = item->data;

			App->render->Blit(atlas, uibutton->rect_position.x - App->render->camera.x, uibutton->rect_position.y - App->render->camera.y, &uibutton->rect_atlas);
		}
		else if (*type == UIComponent_TYPE::UIIMAGE)
		{
			UIComponents* uiimage = item->data;

			App->render->Blit(atlas, uiimage->rect_position.x - App->render->camera.x, uiimage->rect_position.y - App->render->camera.y, &uiimage->rect_atlas);
		}
		else if (*type == UIComponent_TYPE::UIINPUT)
		{
			UIComponents* uiinput = item->data;

			App->render->Blit(atlas, uiinput->rect_position.x - App->render->camera.x, uiinput->rect_position.y - App->render->camera.y, &uiinput->rect_atlas);
		}
		else if (*type == UIComponent_TYPE::UILABEL)
		{
			UILabel* uilabel = (UILabel*)item->data;

			App->render->Blit(uilabel->text_img, uilabel->rect_position.x - App->render->camera.x, uilabel->rect_position.y - App->render->camera.y);
		}
	}
}

// class uimanager ---------------------------------------------------

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

UIButton::UIButton(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UIButton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
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

void UIButton::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

UIInput::UIInput(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UIInput::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
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

void UIInput::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

UIImage::UIImage(UIComponent_TYPE type) : UIComponents(type)
{

}

void UIImage::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
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

void UIImage::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}
