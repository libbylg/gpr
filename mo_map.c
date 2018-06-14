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

#define MO_CAPS_COUNT 29
static const int    mo_caps[] =
{
    23ul,           53ul,           97ul,               193ul,          389ul,        
    769ul,          1543ul,         3079ul,             6151ul,         12289ul,      
    24593ul,        49157ul,        98317ul,            196613ul,       393241ul,     
    786433ul,       1572869ul,      3145739ul,          6291469ul,      12582917ul,   
    25165843ul,     50331653ul,     100663319ul,        201326611ul,    402653189ul,  
    805306457ul,    1610612741ul,   3221225473ul,       4294967291ul    
};

static inline long  mo_tidy_size(int len)
{
    for (long* p = mo_caps; (*p < len) && (p < mo_caps + (MO_CAPS_COUNT - 1)); p++)
    {
    }

    return *p;
}


MO_MAP_EXTERN   struct mo_map_t*        mo_map_new      (int init_cap, MO_MAP_HASH h, MO_MAP_EQUAL q)
{
    if ((init_cap <= 0) || (NULL == h) || (NULL == q))
    {
        return NULL;
    }

    init_cap = mo_tidy_size(init_cap);

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

MO_MAP_EXTERN   struct mo_entry_t*      mo_map_put   (struct mo_map_t* m, struct mo_entry_t* entry)
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

MO_MAP_EXTERN   struct mo_map_node_t*   mo_map_rm   (struct mo_map_t* m, void* key)
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
    index = ((index < 0)?-index:index) % m->cap;
    struct mo_buckets_t* b = m->buckets[index];

    struct mo_entry_t* e = (struct mo_entry_t*)b;
    while (e->next != (struct mo_entry_t*)b)
    {
        if (0 == ((*(m->equal))(key, e->next->key)))
        {
            return e->next;
        }

        e = e->next;
    }

    return NULL;
}

MO_MAP_EXTERN   int                     mo_map_size     (struct mo_map_t* m)
{
    return m->size;
}

MO_MAP_EXTERN   int                     mo_map_resize   (struct mo_map_t* m, int new_cap)
{
    if (new_cap < 0)
    {
        return -1;
    }

    new_cap = mo_tidy_size(new_cap);
    if (new_cap == m->cap)
    {
        return -2;
    }

    struct mo_buckets_t* new_buckets = realloc(m->buckets, new_cap * sizeof(struct mo_buckets_t));
    if (NULL == new_buckets)
    {
        return -3;
    }

    m->buckets = new_buckets;
    return 0;
}

MO_MAP_EXTERN   int            mo_map_string_hash_default(char* s)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    while(*s)  
    {  
        hash = (hash << 4) + *s;  
        if ((x = hash & 0xf0000000) != 0)
        {  
            hash ^= (x >> 24);   //影响5-8位，杂糅一次   
            hash &= ~x;   //清空高四位   
        }  

        s++;  
    }  

}


