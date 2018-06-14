#include "mo_map.h"


#include <stdlib.h>


struct mo_buckets_t
{
    struct mo_entry_t*  next;
    union
    {
        int             count;
        void*           key;
    }
};


struct mo_map_t
{
    int                     size;
    int                     cap;
    MO_MAP_HASH             hash;
    MO_MAP_EQUAL            equal;
    struct mo_buckets_t**   buckets;
};


MO_MAP_EXTERN   struct mo_map_t*        mo_map_new      (int init_cap, MO_MAP_HASH h, MO_MAP_EQUAL q)
{
    if ((init_cap <= 0) || (NULL == hash))
    {
        return NULL;
    }

    if (init_cap < 97)
    {
        init_cap = 97;
    }

    size_t size = init_cap * sizeof(struct mo_buckets_t*);
    struct mo_buckets_t** buckets = (struct mo_buckets_t**)malloc(size);
    if (NULL == m)
    {
        return NULL;
    }
    
    for (int i = 0; i < init_cap; i++)
    {
        buckets[i]->count = 0;
        buckets[i]->next  = (struct mo_entry_t*)(&(buckets[i]));   ///<    形成环状结构
    }

    struct mo_map_t* m = (struct mo_map_t*)malloc(sizeof(mo_map_t));
    if (NULL == buckets)
    {
        free(m);
        return NULL;
    }

    m->size     =   0;
    m->cap      =   init_cap;
    m->hash     =   hash;
    m->buckets  =   buckets;
    return m;
}

MO_MAP_EXTERN   void                    mo_map_del      (struct mo_map_t* m)
{
    if (NULL == m)
    {
        return;
    }

    free(m->buckets);
    free(m);
}

MO_MAP_EXTERN   struct mo_entry_t*      mo_map_insert   (struct mo_map_t* m, struct mo_entry_t* entry)
{
    int index = (*(m->hash))(key) % (m->init_cap);
    struct mo_buckets_t* b = m->buckets[index];

    struct mo_entry_t* e = (struct mo_entry_t*)b;
    while (e->next != (struct mo_entry_t*)b)
    {
        if (0 == ((m->equal)(entry->key, e->next->key)))
        {
            break;
        }

        e = e->next;
    }

    if (e->next == ((struct mo_entry_t*)b)
    {
        entry->next = e->next;
        e->next     = entry;
        b->count++;
        m->size++;
        return NULL;
    }

    struct mo_entry_t* replace = e->next;
    entry->next = replace->next;
    e->next     = entry;
    replace     = NULL;
    return replace;
}

MO_MAP_EXTERN   struct mo_map_node_t*   mo_map_remove   (struct mo_map_t* m, void* key)
{
    int index = (*(m->hash))(key) % (m->init_cap);
    struct mo_buckets_t* b = m->buckets[index];

    struct mo_entry_t* e = (struct mo_entry_t*)b;
    while (e->next != (struct mo_entry_t*)b)
    {
        if (0 == ((m->equal)(key, e->next->key)))
        {
            break;
        }

        e = e->next;
    }

    if (e->next == ((struct mo_entry_t*)b)
    {
        return NULL;
    }

    struct mo_entry_t* removed = e->next;
    e->next         = removed->next;
    removed->next   = NULL;
    return removed;
}

MO_MAP_EXTERN   struct mo_map_node_t*   mo_map_get      (struct mo_map_t* m, void* key)
{
    int index = (*(m->hash))(key) % (m->init_cap);
    struct mo_buckets_t* b = m->buckets[index];

    struct mo_entry_t* e = (struct mo_entry_t*)b;
    while (e->next != (struct mo_entry_t*)b)
    {
        if (0 == ((m->equal)(key, e->next->key)))
        {
            return e->next;
        }

        e = e->next;
    }
}

MO_MAP_EXTERN   int                     mo_map_size     (struct mo_map_t* m)
{
    return m->size;
}

MO_MAP_EXTERN   int                     mo_map_rehash   (struct mo_map_t* m, int new_cap, MO_MAP_HASH hash)
{
    if (new_cap < m->cap)
    {
        return 
    }

}

MO_MAP_EXTERN   unsigned int            mo_map_string_hash_default(char* s)
{

}


