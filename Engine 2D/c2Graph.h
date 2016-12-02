// ----------------------------------------------------
// Graph implementation  ------------
// ----------------------------------------------------

#ifndef __c2Graph_H__
#define __c2Stack_H__

#include "c2Defs.h"
#include "c2List.h"
#include "c2DynArray.h"
#include "c2Queue.h"

/**
* We will use a double linked list
*/
template<class TYPE>
class c2Graph
{
public:

	struct vertice
	{
		TYPE data;
		c2List<vertice*> links;

		vertice(const TYPE& data) : data(data)
		{}


	};

private:

	c2List<vertice*> vertices;

public:
	/* RECODE
	vertice* push_back(const TYPE& data)
	{
		vertice* new_vertice = new vertice(data);
		vertices.push_back(new_vertice);
		return new_vertice;
	}

	void clear()
	{
		const c2List_item<vertice*>* p = vertices.start;
		while(p != nullptr)
		{
			RELEASE(p->data);
			p = p->next();
		}
		vertices.clear();
	}
	*/
};

#endif // __p2Graph_H__