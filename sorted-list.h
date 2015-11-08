#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>


/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return a negative value if the
 * first object is smaller, 0 if the two objects are equal, and a positive
 * value if the second object is smaller.
 */
typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)( void * );


/*
	Linked list node structure.
*/
typedef struct Node
{
	void 			*data;
	int 			pointerCount;
	struct Node 	*next, *prev;
} Node;


/*
 * Sorted list type.  You need to fill in the type as part of your implementation.
 */
struct SortedList
{
	Node* 			head;
	CompareFuncT 	comparator;
	DestructFuncT 	destroyer; 
};
typedef struct SortedList* SortedListPtr;


struct SortedListIterator
{
	SortedListPtr 	list;
	Node 			*iterNode;
	DestructFuncT	destroyer;
};
typedef struct SortedListIterator* SortedListIteratorPtr;



SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);


void SLDestroy(SortedListPtr list);


int SLInsert(SortedListPtr list, void *newObj);


int SLRemove(SortedListPtr list, void *newObj);


SortedListIteratorPtr SLCreateIterator(SortedListPtr list);


void SLDestroyIterator(SortedListIteratorPtr iter);


void * SLGetItem( SortedListIteratorPtr iter );


void * SLNextItem(SortedListIteratorPtr iter);


/* New additions for Indexer*/

void * SLSearch(SortedListPtr list, void *Obj, CompareFuncT);

int SLIsLastItem(SortedListIteratorPtr iter);

int SLisEmpty(SortedListPtr list);



#endif