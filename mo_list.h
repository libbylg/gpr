#ifndef __list_
#define __list_


#ifndef MO_MAP_EXTERN
    #if defined(__cplusplus)
    #define MO_MAP_EXTERN   extern  "C"
    #else
    #define MO_MAP_EXTERN   extern
    #endif
#endif


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

MO_EXTERN   void                mo_list_init(struct mo_list_t* list);
MO_EXTERN   void                mo_list_push_back(struct mo_list_t* list, struct mo_node_t* item);
MO_EXTERN   void                mo_list_push_front(struct mo_list_t* list, struct mo_node_t* item);
MO_EXTERN   struct mo_node_t*   mo_list_pop_front(struct mo_list_t* list);
MO_EXTERN   struct mo_node_t*   mo_list_pop_back(struct mo_list_t* list);
MO_EXTERN   int                 mo_list_count(struct mo_list_t* list);
MO_EXTERN   struct mo_node_t*   mo_list_begin(struct mo_list_t* list);
MO_EXTERN   struct mo_node_t*   mo_list_end(struct mo_list_t* list);
MO_EXTERN   struct mo_node_t*   mo_list_next(struct mo_list_t* list, struct mo_node_t* itr);
MO_EXTERN   struct mo_node_t*   mo_list_prev(struct mo_list_t* list, struct mo_node_t* itr);
#define mo_list_foreach(type,itr,list) \
    for (type itr = (type)(list->next); (struct mo_node_t*)itr != &(list->guard); itr = (type)(itr->next))

#endif//__list_


