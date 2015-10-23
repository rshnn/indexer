
#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"



/*
	Creates and initializes the sorted list structure. 
*/
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){

	SortedListPtr list 	= (SortedListPtr) malloc(sizeof(struct SortedList));

	if(list == NULL){
		return NULL;
	}

	list->head 			= malloc(sizeof(Node));
	list->head->data	= NULL;
	list->head->next 	= list->head;
	list->head->prev 	= list->head; 
	list->comparator	= cf;
	list->destroyer 	= df;

	return list;
}




void SLDestroy(SortedListPtr list){

	if(list != NULL){
		Node *temp 	= list->head->next;

		while(temp != list->head){
			temp->prev->next 	= temp->next;
			temp->next->prev 	= temp->prev;
			list->destroyer(temp->data);
			free(temp);
			temp = list->head->next;
		}

		free(list->head);
		free(list);
	}
}





/* 
 	SLInset will insert a new object into the sorted list.
 		Sorted order is maintained with the comparator function.
 		Ignores duplicates.
 	If the function succeeds, it returns 1, otherwise it returns 0.
 */

int SLInsert(SortedListPtr list, void *newObj){

	Node *temp 	= list->head->next;

	while(temp != list->head){

		if(list->comparator(newObj, temp->data) > 0){
			break;
		}
		temp = temp->next;
	}

	Node *newNode = malloc(sizeof(Node));
	if(newNode == NULL){
		exit(0);
	}

	newNode->pointerCount = 0;
	newNode->data 			= newObj;
	newNode->prev 			= temp->prev;
	temp->prev->next 		= newNode;
	newNode->next 			= temp;
	temp->prev 				= newNode;

	return 1;
}



/*
 	SLRemove will remove a given Object from the sorted list.
 		Ordering is maintained.  Does not change the object pointer.
 
 	If the function succeeds, it returns 1, otherwise it returns 0.
 */
int SLRemove(SortedListPtr list, void *newObj){
	
	Node *temp = list->head->next;

	while(temp != list->head){
		if(list->comparator(newObj, temp->data) == 0){
			break;
		}

		temp = temp->next;
	}
	if(temp == list->head){
		printf("Error:  Attempting to remove an object that does not exist.\n");
		return 0;
	}

	temp->next->prev 	= temp->prev;
	temp->prev->next 	= temp->next;
	if(temp->pointerCount == 0){
		list->destroyer(temp->data);
		free(temp);
	}

	temp->next = NULL;
	temp->prev = NULL;
	return 1;



}




/*
 	SLCreateIterator initializes the iterator structure.
		Points to the first item of the sorted list if non-empty.
		If sorted list is empty, returns null pointer.
 	Returns a non-NULL pointer to the iterator structure.  Otherewise, NULL.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){

	if(list == NULL){
		/* Sorted List structure DNE. */
		return NULL;
	}
	if(list->head == NULL){
		/* Sorted list is initialized, but empty. */
		return NULL;
	}
	SortedListIteratorPtr iterator 	= (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	iterator->list 		= list;
	iterator->iterNode	= list->head->next;
	iterator->destroyer	= list->destroyer;
	iterator->iterNode->pointerCount++;
	return iterator;
}





/*
	SLDestroyIterator frees the iterator structure.  Sorted list unaffected.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){

	if(iter == NULL){
		printf("Error: Iterator object does not exist.\n");
		return;
	}
	if(iter->iterNode->pointerCount > 0){
		iter->iterNode->pointerCount--;
	}
	if(iter->iterNode->pointerCount == 0 && iter->iterNode->next == NULL){
		iter->list->destroyer(iter->iterNode->data);
		free(iter->iterNode);
	}
	free(iter);

}




/*
	SLGetItem returns a void pointer to the data associated with iterNode.
		Will return 0 if iterator advances out of bounds or if iterator DNE.
*/

void * SLGetItem( SortedListIteratorPtr iter ){

	if(iter == NULL){
		printf("Error: Iterator object does not exist.\n");
		return NULL;
	}
	if(iter->iterNode->next == NULL){
		return 0;
	}
	return iter->iterNode->data;
}




/*

 	SLNextItem increments the iterator and returns data of the next node.
 	Returns NULL when at end of list.

 */

void * SLNextItem(SortedListIteratorPtr iter){

	if(iter == NULL){
		printf("Error: Iterator object does not exist.\n");
		return NULL;
	}
	if(iter->iterNode->next == NULL){
		return NULL;
	}


	if(iter->iterNode->next != iter->list->head){
		iter->iterNode->pointerCount--;
		iter->iterNode = iter->iterNode->next;
		iter->iterNode->pointerCount++;
	}else
		return NULL;

	return SLGetItem(iter);

}



/* New functions for indexer */



void * SLSearch(SortedListPtr list, void *Obj, CompareFuncT cf)
{
	Node *temp 	= list->head->next;

	while(temp != list->head){
		if(cf(Obj, temp->data) == 0){
			break;
		}
		temp = temp->next;
	}
	return temp->data;

	// if(success)	
	// 	return ptr->data;
	// else 
	// 	return NULL;

}





int SLIsLastItem(SortedListIteratorPtr iter)
{
  if (iter->iterNode->next == iter->list->head)
    return 1;
  else
    return 0;
}


















