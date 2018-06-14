#ifndef __list_
#define __list_

struct mo_node_t
{
    struct mo_node_t* prev;
    struct mo_node_t* next;
};


struct mo_list_t
{
    struct mo_node_t    guard;
    int                 count;
};

void                mo_list_init(struct mo_list_t* list);
void                mo_list_push_back(struct mo_list_t* list, struct mo_node_t* item);
void                mo_list_push_front(struct mo_list_t* list, struct mo_node_t* item);
struct mo_node_t*   mo_list_pop_front(struct mo_list_t* list);
struct mo_node_t*   mo_list_pop_back(struct mo_list_t* list);
int                 mo_list_count(struct mo_list_t* list);
struct mo_node_t*   mo_list_begin(struct mo_list_t* list);
struct mo_node_t*   mo_list_end(struct mo_list_t* list);
struct mo_node_t*   mo_list_next(struct mo_list_t* list, struct mo_node_t* itr);
struct mo_node_t*   mo_list_prev(struct mo_list_t* list, struct mo_node_t* itr);
#define mo_list_foreach(type,itr,list) \
    for (type itr = list->next; itr != &(list->guard); itr = itr->next)

#endif//__list_


