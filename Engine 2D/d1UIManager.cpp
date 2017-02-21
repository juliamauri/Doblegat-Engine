#define RIGHT_CLICK 3
#define LEFT_CLICK 1

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

		if (component->draw)
		{
			if ((x_mouse > component->rect_position.x) && (x_mouse < component->rect_position.x + component->rect_position.w) && (y_mouse > component->rect_position.y) && (y_mouse < component->rect_position.y + component->rect_position.h))
			{
				component->stat = UIComponent_Stat::SELECTED;

				if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UIComponent_Stat::SELECTED)
					{
						component->stat = UIComponent_Stat::CLICKL_DOWN;

						//Check or uncheck for UICheckbutton
						if (component->type == UIComponent_TYPE::UICHECKBUTTON)
						{
							UICheckbutton* button_checked = (UICheckbutton*)component;
							if (button_checked->clicked)
								button_checked->clicked = false;
							else
								button_checked->clicked = true;
						}

						//Expand options from UISelectOption
						if (component->type == UIComponent_TYPE::UISELECTOPTION)
						{
							UISelectOption* select_option = (UISelectOption*)component;
							select_option->selecting = true;
							select_option->ChangeDrawAllOptions();
						}

						//Selecting option from UISelectOption
						if (component->type == UIComponent_TYPE::UILABEL && (component->from != nullptr && component->from->type == UIComponent_TYPE::UISELECTOPTION))
						{
							UISelectOption* from_option_selected = (UISelectOption*)component->from;
							UILabel* option_selected = (UILabel*)component;

							if (option_selected != from_option_selected->current)
							{
								from_option_selected->ChangeCurrent(option_selected);
								from_option_selected->ChangeDrawAllOptions();
								from_option_selected->selecting = false;
							}
						}
					}
					else
						component->stat = UIComponent_Stat::CLICKL_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(LEFT_CLICK) == KEY_UP)
				{
					if (component->stat == UIComponent_Stat::CLICKL_REPEAT || component->stat == UIComponent_Stat::CLICKL_DOWN)
						component->stat = UIComponent_Stat::CLICKL_UP;
					else
						component->stat = UIComponent_Stat::SELECTED;
				}

				if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_DOWN)
				{
					if (component->stat == UIComponent_Stat::SELECTED)
						component->stat = UIComponent_Stat::CLICKR_DOWN;
					else
						component->stat = UIComponent_Stat::CLICKR_REPEAT;
				}
				else if (App->input->GetMouseButtonDown(RIGHT_CLICK) == KEY_UP)
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
	else if(type == UIComponent_TYPE::UICHECKBUTTON)
	{
		components.add(ret = new UICheckbutton(UIComponent_TYPE::UICHECKBUTTON));
	}
	else if (type == UIComponent_TYPE::UISELECTOPTION)
	{
		components.add(ret = new UISelectOption(UIComponent_TYPE::UISELECTOPTION));
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
		if (item->data->draw)
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
			else if (*type == UIComponent_TYPE::UICHECKBUTTON)
			{
				UICheckbutton* uicheckbutton = (UICheckbutton*)item->data;

				if (uicheckbutton->clicked)
					App->render->Blit(atlas, uicheckbutton->rect_position.x - App->render->camera.x, uicheckbutton->rect_position.y - App->render->camera.y, &uicheckbutton->rect_atlas_clicked);
				else
					App->render->Blit(atlas, uicheckbutton->rect_position.x - App->render->camera.x, uicheckbutton->rect_position.y - App->render->camera.y, &uicheckbutton->rect_atlas);
			}
			else if (*type == UIComponent_TYPE::UISELECTOPTION)
			{
				UISelectOption* uiselectoption = (UISelectOption*)item->data;

				App->render->Blit(atlas, uiselectoption->rect_position.x - App->render->camera.x, uiselectoption->rect_position.y - App->render->camera.y, &uiselectoption->rect_atlas);

				if (uiselectoption->selecting)
				{
					for(uint i = 1; i < uiselectoption->num_options + 1; i++)
						App->render->Blit(atlas, uiselectoption->rect_position.x - App->render->camera.x, (uiselectoption->rect_position.y - App->render->camera.y) + uiselectoption->rect_atlas.h * i, &uiselectoption->rect_atlas);
				}
			}
		}
	}
}

// class uimanager ---------------------------------------------------

void UILabel::Set(int pos_x, int pos_y, const char * text, _TTF_Font*  font)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;

	text_img = App->font->Print(text, { 255,0,0,0 });

	this->text.create(text);

	App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);
}

void UILabel::ChangeText(const char* text)
{
	if (this->text != text)
	{
		text_img = App->font->Print(text, { 255,0,0,0 });

		App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);

		this->text.Clear();
		this->text.create(text);
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

UICheckbutton::UICheckbutton(UIComponent_TYPE type) : UIButton(type) 
{

}

void UICheckbutton::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h, uint atlas_clicked_x, uint atlas_clicked_y, uint atlas_clicked_w, uint atlas_clicked_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;

	rect_atlas_clicked.x = atlas_clicked_x;
	rect_atlas_clicked.y = atlas_clicked_y;
	rect_atlas_clicked.w = atlas_clicked_w;
	rect_atlas_clicked.h = atlas_clicked_h;
}

void UICheckbutton::Set(const SDL_Rect & position, const SDL_Rect & atlas, const SDL_Rect & atlas_clicked)
{
	rect_position = position;
	rect_atlas = atlas;
	rect_atlas_clicked = atlas_clicked;
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

UISelectOption::UISelectOption(UIComponent_TYPE type) : UIComponents(type)
{
	title = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);
}

void UISelectOption::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
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

void UISelectOption::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UISelectOption::AddOption(const char * text)
{
	UILabel* newoption = (UILabel*)App->uimanager->addUIComponent(UIComponent_TYPE::UILABEL);

	num_options++;

	newoption->Set(rect_position.x + 2 , (rect_position.y + (rect_position.h/4)) + (num_options*rect_position.h), text);

	newoption->draw = false;

	newoption->from = this;

	if (current == nullptr)
		ChangeCurrent(newoption);
	else
		options.add(newoption);
}

const char * UISelectOption::CheckSelected()
{
	return current->text.GetString();
}

void UISelectOption::ChangeCurrent(UILabel * change)
{
	if (current == nullptr)
	{
		current = change;
		current->rect_position.x = rect_position.x;
		current->rect_position.y = rect_position.y;
		current->draw = true;
	}
	else
	{
		options.del(options.At(options.find(change)));
		
		SWAP<int>(current->rect_position.x, change->rect_position.x);
		SWAP<int>(current->rect_position.y, change->rect_position.y);

		options.add(current);


		current = change;
		current->draw = true;
	}
}

void UISelectOption::ChangeDrawAllOptions()
{
	c2List_item<UILabel*>* item = options.start;

	for (; item != NULL; item = item->next)
	{
		if (item->data->draw)
			item->data->draw = false;
		else
			item->data->draw = true;
	}
}
