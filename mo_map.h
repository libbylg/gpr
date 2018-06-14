#ifndef __mo_map_
#define __mo_map_

#ifndef MO_MAP_EXTERN
    #if defined(__cplusplus)
    #define MO_MAP_EXTERN   extern  "C"
    #else
    #define MO_MAP_EXTERN   extern
    #endif
#endif


struct mo_entry_t
{
    struct mo_entry_t*   next;
    void*                key;
};

struct mo_map_t;

typedef int     (*MO_MAP_HASH)(void* key);
typedef int     (*MO_MAP_EQUAL)(void* key1, void* key2);
MO_MAP_EXTERN   struct mo_map_t*        mo_map_new      (int init_cap, MO_MAP_HASH h, MO_MAP_EQUAL q);
MO_MAP_EXTERN   void                    mo_map_del      (struct mo_map_t* m);
MO_MAP_EXTERN   int                     mo_map_insert   (struct mo_map_t* m, struct mo_entry_t* entry);
MO_MAP_EXTERN   struct mo_entry_t*      mo_map_remove   (struct mo_map_t* m, void* key);
MO_MAP_EXTERN   struct mo_entry_t*      mo_map_get      (struct mo_map_t* m, void* key);
MO_MAP_EXTERN   int                     mo_map_size     (struct mo_map_t* m);
MO_MAP_EXTERN   int                     mo_map_rehash   (struct mo_map_t* m, int new_cap, MO_MAP_HASH hash);

MO_MAP_EXTERN   unsigned int            mo_map_string_hash_default(char* s);

#endif//__mo_map_
