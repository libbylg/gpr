#include "mo_map.h"



struct mo_map_t
{
    int                     size;
    int                     cap;
    MO_MAP_HASH             hash;
    struct mo_map_node_t*   buckets;
};


MO_MAP_EXTERN   struct mo_map_t*        mo_map_new      (int init_cap, MO_MAP_HASH hash)
{

}

MO_MAP_EXTERN   void                    mo_map_del      (struct mo_map_t* m)
{

}

MO_MAP_EXTERN   int                     mo_map_insert   (struct mo_map_t* m, void* key, struct mo_map_node_t*  value)
{

}

MO_MAP_EXTERN   struct mo_map_node_t*   mo_map_remove   (struct mo_map_t* m, void* key)
{

}

MO_MAP_EXTERN   struct mo_map_node_t*   mo_map_get      (struct mo_map_t* m, void* key)
{

}

MO_MAP_EXTERN   int                     mo_map_size     (struct mo_map_t* m)
{

}

MO_MAP_EXTERN   int                     mo_map_rehash   (struct mo_map_t* m, int new_cap, MO_MAP_HASH hash)
{

}

MO_MAP_EXTERN   unsigned int            mo_map_string_hash_default(char* s)
{

}


