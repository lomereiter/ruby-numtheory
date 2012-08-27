#ifndef NUMTHEORY_QUEUE_H
#define NUMTHEORY_QUEUE_H

#include <stdlib.h>

typedef struct Node Node;
struct Node {
    void* val;
    Node* next;
};

typedef struct {
    Node* first;
    Node* last;
} Queue;

static inline Queue* queue_new()
{
    Queue* q;
    q = malloc(sizeof(Queue));
    q -> first = q -> last = NULL;
    return q;
}

static inline void queue_push(Queue* q, void* elem)
{
    Node* new_node = malloc(sizeof(Node));
    new_node -> val = elem;
    new_node -> next = NULL;
    if (q -> last)
    {
        q -> last -> next = new_node;
    }
    else
    {
        q -> first = new_node;
    }
    q -> last = new_node;
}

static inline void* queue_pop(Queue* q)
{
    if (q -> first == NULL)
    {
        return NULL;
    }
    Node* fst_node = q -> first;
    void* elem = fst_node -> val;
    q -> first = fst_node -> next;
    if (q -> first == NULL)
    {
        q -> last = NULL;
    }
    free(fst_node);
    return elem;
}

static inline int queue_empty(Queue* q)
{
    return q -> first == NULL;
}

#endif
