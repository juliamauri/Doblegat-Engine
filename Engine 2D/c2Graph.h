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

		bool CanReach(const vertice* dst, c2DynArray<const vertice*>& visited) const
		{
			// Are we the destination ?
			if(this == dst)
				return true;

			// Add ourselves as a visited node so we never come back here
			visited.PushBack(this);
	
			// Recursively call all our links that have not been visited
			for(const c2List_item<vertice*>* item = links.start; item; item = item->next())
			{
				if(visited.find(item->data) == visited.size())
				{
					if(item->data->CanReach(dst, visited) == true)
						return true;
				}
			}

			// We could not find destination in this branch
			return false;
		}
	};

private:

	c2List<vertice*> vertices;

public:

	vertice* push_back(const TYPE& data)
	{
		vertice* new_vertice = new vertice(data);
		vertices.push_back(new_vertice);
		return new_vertice;
	}

	bool is_reachable_iterative(const vertice* src, const vertice* dst) const
	{
		if(src == dst)
			return true;

		// Keep track of visited nodes
		c2DynArray<const vertice*> visited(size());

		// queue to organize node visiting
		c2Queue<const vertice*> queue;

		// Add first node as visited
		visited.PushBack(src);
		
		// Add first node as the one to begin with
		queue.Push(src);

		const vertice* current;
		while(queue.Pop(current) == true)
		{
			
			// Iterate all links
			const c2List_item<vertice*>* item = current;
			for(; item != nullptr; item = item->next())
			{
				// If we found destination, we are finish
				if(item->data == dst)
					return true;

				// Check if is already visited
				if(visited.find(item->data) == visited.size())
				{
					// If not mark is as visited and add it to the queue
					visited.PushBack(item->data);
					queue.Push(item->data);
				}
			}
		}

		return false;
	}

	bool is_reachable_recursive(const vertice* src, const vertice* dst) const
	{
		// Keep track of visited nodes
		c2DynArray<const vertice*> visited(size());

		// trigger the recursive function
		return src->CanReach(dst, visited);
	}



	unsigned int size() const
	{
		return vertices.size();
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

	bool empty() const
	{
		return vertices.empty();
	}

};

#endif // __p2Graph_H__