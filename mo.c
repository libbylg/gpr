#include "mo.h"


struct lex_t
{
    void*               ctx;
    struct lex_t*       parent;
    struct result_t*    result;
    mo_u32              (*next)(void* ctx, struct lex_t*  x, struct token_t* t);
};

struct unit_t
{
    void*               ctx;
    struct lex_t*       parent;
    struct result_t*    result;
    mo_u32              (*accept)(void* ctx, struct unit_t* x, struct token_t* t)
};

struct mo_t
{
    void*               ctx;
    struct lex_t*       lex;
    struct unit_t*      unit;
    struct result_t*    result;
};




MO_EXTERN   struct mo_t*        mo_new(void* ctx, struct lex_t* x, struct unit_t* u, struct result_t* r)
{
    struct mo_t* m = (struct mo_t*)malloc(sizeof(struct mo_t));
    if (NULL == m)
    {
        return NULL;
    }

    m->lex    = x;
    m->unit   = u;
    m->result = r;
    m->ctx    = ctx;
    return m;
}




MO_EXTERN   struct lex_t*       mo_lex_new (void* ctx, mo_u32 (*next  )(void* ctx, struct lex_t*  x, struct token_t* t), struct result_t* r)
{
    struct lex_t* x = (struct lex_t*)malloc(sizeof(struct lex_t));
    if (NULL == x)
    {
        return NULL;
    }

    x->parent = NULL;
    x->ctx    = ctx;
    x->next   = next;
    x->result = r;
    return x;
}




MO_EXTERN   struct unit_t*      mo_unit_new(void* ctx, mo_u32 (*accept)(void* ctx, struct unit_t* x, struct token_t* t), struct result_t* r)
{
    struct unit_t* u = (struct unit_t*)malloc(sizeof(struct unit_t));
    if (NULL == u)
    {
        return NULL;
    }

    u->parent = NULL;
    u->ctx    = ctx;
    u->accept = accept;
    u->result = r;
    return u;
}




MO_EXTERN   struct result_t*    mo_result(struct unit_t* x);
MO_EXTERN   struct result_t*    mo_lex_result (struct lex_t*  x);
MO_EXTERN   struct result_t*    mo_unit_result(struct unit_t* x);



MO_EXTERN   void                mo_setup_result(struct mo_t* m, struct result_t* r);
MO_EXTERN   struct unit_t*      mo_compile(struct mo_t* m, void* ctx, int (*read)(char* data, int* len));
MO_EXTERN   void                mo_del(struct mo_t* m);



MO_EXTERN   struct mo_t*        mo_push(struct mo_t* m, struct unit_t* u);
MO_EXTERN   struct unit_t*      mo_pop(struct mo_t* m);