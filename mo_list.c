#include "mo_list.h"



#ifndef NULL
#if defined(__cplusplus)
#define NULL    (void*)(0)
#else
#define NULL    (0)
#endif
#endif


void    mo_list_init(struct mo_list_t* list)
{
    list->count      = 0;
    list->guard.next = &(list->guard);
    list->guard.prev = &(list->guard);
}

void    mo_list_push_back(struct mo_list_t* list, struct mo_node_t* item)
{
    struct mo_node_t* last = list->guard.prev;
    item->next = &(list->guard);
    item->prev = last;
    last->next = item;
    list->guard.prev = item;
    list->count++;
}

void    mo_list_push_front(struct mo_list_t* list, struct mo_node_t* item)
{
    struct mo_node_t* first = list->guard.next;
    item->prev = &(list->guard);
    item->next = first;
    first->prev = item;
    list->guard.next = item;
    list->count++;
}

struct mo_node_t*   mo_list_pop_front(struct mo_list_t* list)
{
    struct mo_node_t* first = list->guard.next;
    if (first == &(list->guard))
    {
        return NULL;
    }

    first->next->prev = first->prev;
    list->guard.next = first->next;
    first->prev = NULL;
    first->next = NULL;

    list->count--;
    return first;
}

struct mo_node_t*   mo_list_pop_back(struct mo_list_t* list)
{
    struct mo_node_t* last = list->guard.prev;
    if (last == &(list->guard))
    {
        return NULL;
    }

    last->prev->next = last->next;
    list->guard.prev = last->prev;
    last->prev = NULL;
    last->next = NULL;

    list->count--;
    return last;
}

int mo_list_count(struct mo_list_t* list)
{
    return list->count;
}

struct mo_node_t*   mo_list_begin(struct mo_list_t* list)
{
    if (list->guard.next == &(list->guard))
    {
        return NULL;
    }

    return list->guard.next;
}

struct mo_node_t*   mo_list_end(struct mo_list_t* list)
{
    if (list->guard.prev == &(list->guard))
    {
        return NULL;
    }

    return list->guard.prev;
}

struct mo_node_t*   mo_list_next(struct mo_list_t* list, struct mo_node_t* itr)
{
    if (itr->next == &(list->guard))
    {
        return NULL;
    }

    return itr->next;
}

struct mo_node_t*   mo_list_prev(struct mo_list_t* list, struct mo_node_t* itr)
{
    if (itr->prev == &(list->guard))
    {
        return NULL;
    }

    return itr->prev;
}



