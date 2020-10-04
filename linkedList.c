/**
 * Re-used from Practical 7
 * Implementation of a singly-linked double-ended generic linked list.
 *
 * Author: Lachlan Mackenzie
 * Date Last Modified: 2018-10-15
 */

#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/* These two methods should be limited to this Linked List file */
static Node* LinkedList_createNode(void* data);

static void LinkedList_freeNode(Node* node, void (* freeData)(void*));

/**
 * Allocates enough memory for an empty Linked List and initialises all fields to
 * their default values and returns the Linked List.
 *
 * Returns:
 *  list - a default Linked List
 */
LinkedList* LinkedList_create()
{
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/**
 * A private function for allocating memory to a Node and initialising it to
 * default values and then returns the Node.
 *
 * Parameters:
 *  data - a void pointer to the data that will be stored in the Node
 * Returns:
 *  node - a default Node
 */
static Node* LinkedList_createNode(void* data)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;

    return node;
}

/**
 * Returns true(non-zero)if the Linked List is empty, i.e. the head of the
 * Linked List is NULL.
 *
 * Parameters:
 *  list - the Linked List to test for emptiness
 * Returns:
 *  true(non-zero) if the Linked List is empty, false(zero) if otherwise
 */
int LinkedList_isEmpty(LinkedList* list)
{
    return list->head == NULL;
}

/**
 * Creates a Node with the imported data inside it and appends it to the Linked
 * List at the end.
 *
 * Parameters:
 *  list - the Linked List to append the Node to
 *  data - the data to have a Node hold
 */
void LinkedList_insertLast(LinkedList* list, void* data)
{
    Node* newNode = LinkedList_createNode(data);

    if (LinkedList_isEmpty(list))
    {
        list->head = list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    (list->size)++;
}

/**
 * Removes the Node at the start of the Linked List but retains access to the data
 * inside the Node for the user to possibly free later.
 *
 * Parameters:
 *  list - the Linked List to remove the first element from
 * Returns:
 *  data - the first Node's data, or NULL if the Linked List is empty
 */
void* LinkedList_removeFirst(LinkedList* list)
{
    Node* oldNode = NULL;
    void* data = LinkedList_get(list, 0);

    if (!LinkedList_isEmpty(list))
    {
        oldNode = list->head;
        list->head = oldNode->next;
        (list->size)--;
        /* Do not free the data inside the node as it is being returned. */
        free(oldNode);
        oldNode = NULL;
    }
    if (LinkedList_isEmpty(list))
    {
        list->tail = NULL;
    }

    return data;
}

/**
 * Retrieves the data from a Node at a certain index. The function iterates along
 * the Linked List until the number of Nodes passed is equal to the index--.
 *
 * Parameters:
 *  list  - the Linked List to retrieve the data from an index
 *  index - the index of the Node to retrieve the data from
 * Returns:
 *  data - the data from the Node at the index, or NULL if the Linked List is empty
 */
void* LinkedList_get(LinkedList* list, int index)
{
    Node* node;
    int count = 0;
    void* data = NULL;

    node = list->head;
    /* Iterates along the Linked List until it reaches the correct Node */
    while (node != NULL && count < index)
    {
        node = node->next;
        count++;
    }

    if (node != NULL)
    {
        data = node->data;
    }

    return data;
}

/**
 * Iterates along the Linked List and frees each previous Node as it goes to the
 * next one. The data inside the Node is also free'd using a function pointer
 * provided by the user to maintain a generic Linked List.
 *
 * Parameters:
 *  list     - the Linked List to free
 *  freeData - the function pointer which frees the data inside the node
 */
void LinkedList_free(LinkedList* list, void (* freeData)(void*))
{
    Node* next;
    Node* node = list->head;
    while (node != NULL)
    {
        next = node->next;
        LinkedList_freeNode(node, freeData);
        node = next;
    }
    free(list);
    list = NULL;
}

/**
 * Frees the data inside the Node through the use of a function pointer as well
 * as the Node itself.
 *
 * Parameters:
 *  node     - the Node to free
 *  freeData - the function pointer to free the data inside the Node
 */
static void LinkedList_freeNode(Node* node, void (* freeData)(void*))
{
    (*freeData)(node->data);
    free(node);
    node = NULL;
}
