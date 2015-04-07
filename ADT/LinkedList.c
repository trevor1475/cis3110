//Trevor Livingston 0833394
#include "LinkedList.h"

/*
 *PRE: n/a
 *
 *POST: Returns a new linked list object on success, NULL otherwise 
 *(memory allocation failure) 
 */
LinkedList * LL_create(char name)
{
	LinkedList *new;
    Process *newProcess;
    
    new = malloc(sizeof(LinkedList));
    if(!new)
    {
        return NULL;
    }
    new->next = NULL;

    newProcess = malloc(sizeof(Process));
    if(!newProcess)
    {
        return NULL;
    }
    newProcess->id = name;

    new->p = newProcess;

    return new;
}

/*
 *PRE: n/a
 *
 *POST: Returns a new linked list object on success, NULL otherwise 
 *(memory allocation failure) 
 */
LinkedList * LL_createCopy(Process *temp)
{
    LinkedList *new;
    
    new = malloc(sizeof(LinkedList));
    if(!new)
    {
        return NULL;
    }
    new->next = NULL;
    new->p = temp;
    return new;
}

/*
 *PRE: n/a
 *
 *POST: all memory associated with the linked list is freed
 */
void LL_deleteList(LinkedList *current)
{   
    LinkedList *temp;
    
    while(current != NULL)
    {
        temp = current->next;
        LL_delete(current);
        current = temp;
    }
}

/*
 *PRE: n/a
 *
 *POST: all memory associated with the linked list node is freed
 */
void LL_delete(LinkedList *toBeDeleted)
{
    toBeDeleted->next = NULL;
    free(toBeDeleted->p);
    free(toBeDeleted);
}

/*
 *PRE: n/a
 *
 *POST: all memory associated with the linked list is freed
 */
void LL_deleteCopyList(LinkedList *current)
{   
    LinkedList *temp;
    
    while(current != NULL)
    {
        temp = current->next;
        LL_deleteCopy(current);
        current = temp;
    }
}

/*
 *PRE: n/a
 *
 *POST: all memory associated with the linked list node is freed
 */
void LL_deleteCopy(LinkedList *toBeDeleted)
{
    toBeDeleted->next = NULL;
    toBeDeleted->p->uses = 0;
    free(toBeDeleted);
}

/*
 *PRE: listHead is a valid Linkedlist, and is at the head of the list.
 *
 *POST: Returns the new head of the list
 */
LinkedList * LL_addToFront(LinkedList *listHead, LinkedList *toBeAdded)
{
    if(listHead == NULL)
    {
        return toBeAdded;
    }
    toBeAdded->next = listHead;
    listHead = toBeAdded;
    return listHead;
}

/*
 *PRE: listHead is a valid LinkedList, and is at the head of the list.
 *
 *POST: Returns the head of the list with the appended burst
 */
LinkedList * LL_addToBack(LinkedList *listHead, LinkedList *toBeAdded)
{
    LinkedList *temp = NULL;
    if(listHead == NULL)
    {
        return toBeAdded;
    }

    temp = listHead;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = toBeAdded;
    return listHead;
}

void LL_printList(LinkedList *current)
{
    LinkedList *temp = NULL;
    int count = 0;

    temp = current;
    while(temp != NULL)
    {
        count++;
        printf("%d. %c: %d\n", count, temp->p->id, temp->p->size);
        temp = temp->next;
    }
}