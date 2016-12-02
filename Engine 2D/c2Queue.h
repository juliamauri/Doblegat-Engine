// ----------------------------------------------------
// Prority FIFO implementation with single linked list  -------
// ----------------------------------------------------

#ifndef __c2PQueue_H__
#define __c2PQueue_H__

#include "c2Defs.h"

/**
* Contains items from double linked list
*/
template<class tdata>
struct c2Queue_item
{
	tdata					data;
	c2List<uint>		priority;
	c2Queue_item<tdata>*   next = nullptr;

	c2Queue_item(const tdata& _data,const c2DynArray<uint>& priority) : data(_data), priority(priority)
	{}
};

/**
* Manages a double linked list
*/
template<class tdata>
class c2Queue
{

public:

	c2Queue_item<tdata>*   start;

public:

	/**
	* Constructor
	*/
	inline c2Queue() : start(NULL)
	{}

	/**
	* Destructor
	*/
	~c2Queue()
	{
		Clear();
	}

	/**
	* Get Size
	*/
	unsigned int Count() const
	{
		unsigned int result = 0;
		c2Queue_item<tdata>* tmp;

		for (tmp = start; tmp != NULL; tmp = tmp->next)
			++result;

		return result;
	}

	/**
	* Get last item
	*/
	c2Queue_item<tdata>* GetLast()
	{
		c2Queue_item<tdata>* tmp = start;

		while (tmp != NULL && tmp->next != NULL)
			tmp = tmp->next;

		return tmp;
	}

	/**
	* push new item
	*/
	void Push(const tdata& item,const c2DynArray<uint>& priority)
	{
		c2Queue_item<tdata>*   p_data_item;
		p_data_item = new c2Queue_item < tdata >(item, priority);

		c2Queue_item<tdata>* last = GetLast();

		if (last == NULL)
		{
			start = p_data_item;
		}
		else
		{
			c2Queue_item<tdata>* prev = start;
			c2Queue_item<tdata>* tmp = start;

			uint indexes = priority.Count();

			for (uint i = 0; i < indexes; i++)
			{
				if (tmp == nullptr)
					break;

				if (tmp->priority.Count() < i + 1)
					continue;
				
				while (tmp && tmp->priority[i] <= priority[i] && tmp->priority.Count() <= i + 1)
				{
					prev = tmp;
					tmp = tmp->next;
				}
			}

			p_data_item->next = tmp;
			if (tmp == start)
				start = p_data_item;
			else
				prev->next = p_data_item;
		}
	}

	/**
	* Deletes an item from the list
	*/
	bool Pop(tdata& item)
	{
		bool result = false;

		if (start != nullptr)
		{
			c2Queue_item<tdata>* new_start = start->next;
			item = start->data;
			RELEASE(start);
			start = new_start;
			result = true;
		}

		return result;
	}

	/**
	* Reads one of the values
	*/
	const tdata* Peek(unsigned int index) const
	{
		unsigned int i = 0;
		c2Queue_item<tdata>* tmp;

		for (tmp = start; tmp != NULL && i < index; tmp = tmp->next)
			++i;

		if (tmp != NULL)
			return &(tmp->data);

		return NULL;
	}

	/**
	* Destroy and free all mem
	*/
	void Clear()
	{
		c2Queue_item<tdata>*   p_data;
		c2Queue_item<tdata>*   p_next;
		p_data = start;

		while (p_data != NULL)
		{
			p_next = p_data->next;
			RELEASE(p_data);
			p_data = p_next;
		}

		start = NULL;
	}

};
#endif /*__p2PQueue_H__*/	

