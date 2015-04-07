//Trevor Livingston 0833394
#include "Boolean.h"

typedef struct Param_t
{
	char id;
	int size;
	int uses;
	int location;
	int end;
} Process;

typedef struct LinkedList_t
{
	Process *p;
	struct LinkedList_t *next;
} LinkedList;

LinkedList * 	LL_create(char name);
void 			LL_deleteList(LinkedList *current);
void 			LL_delete(LinkedList *toBeDeleted);
LinkedList * 	LL_createCopy(Process *temp);
void 			LL_deleteCopyList(LinkedList *current);
void 			LL_deleteCopy(LinkedList *toBeDeleted);
LinkedList * 	LL_addToFront(LinkedList *listHead, LinkedList *toBeAdded);
LinkedList *	LL_addToBack(LinkedList *listHead, LinkedList *toBeAdded);
void 			LL_printList(LinkedList *current);
