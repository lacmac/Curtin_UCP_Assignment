#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * A struct representing a singly-linked Node. The data within the Node is a void
 * pointer as to make the Linked List generic. This also means that the data
 * has the be malloc'ed and free'd outside of the Linked List by the user.
 */
typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

/**
 * A struct representing a singly-linked double-ended Linked List containing a
 * number of Node structs. The list is double-ended as to make insertLast much
 * easier and quicker. Using insertLast keeps the list in the correct order and
 * not backwards. A size field is used for instant access to the number of Nodes
 * in the list.
 */
typedef struct
{
    Node* head;
    Node* tail;
    int size;
} LinkedList;

LinkedList* LinkedList_create();

int LinkedList_isEmpty(LinkedList* list);

void LinkedList_insertLast(LinkedList* list, void* data);

void* LinkedList_removeFirst(LinkedList* list);

void* LinkedList_get(LinkedList* list, int index);

void LinkedList_free(LinkedList* list, void (* freeData)(void*));

#endif
