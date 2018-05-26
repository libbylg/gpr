#include "mo.h"

struct stream_t
{
    struct stream_t*    parent;
    void*               ctx;
    int                 (*read)(void* stream, char* data, int* len);
};

struct unit_t
{
    struct unit_t*      parent;
    void*               ctx;
    mo_u32              (*accept)(void* ctx, struct sytx_t* x, struct token_t* t);
};




struct lex_t
{
    void*               ctx;
    struct result_t*    result;
    mo_u32              (*next)(void* ctx, struct lex_t*  x, struct token_t* t);

    struct stream_t*    stream_top;
};

struct sytx_t
{
    void*               ctx;
    struct result_t*    result;
    struct unit_t*      unit_top;
};

struct mo_t
{
    void*               ctx;
    struct lex_t*       lex_top;
    struct sytx_t*      sytx_top;
    struct result_t*    result;
};




MO_EXTERN   struct mo_t*        mo_new(void* ctx, struct lex_t* x, struct sytx_t* u, struct result_t* r)
{
    struct mo_t* m = (struct mo_t*)malloc(sizeof(struct mo_t));
    if (NULL == m)
    {
        return NULL;
    }

    m->lex    = x;
    m->unit   = u;
    m->ctx    = ctx;

    m->result = r;
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




MO_EXTERN   struct sytx_t*      mo_unit_new(void* ctx, mo_u32 (*accept)(void* ctx, struct sytx_t* x, struct token_t* t), struct result_t* r)
{
    struct sytx_t* u = (struct sytx_t*)malloc(sizeof(struct sytx_t));
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


static      struct lex_t*       mo_lex_push(struct lex_t* x, void* stream, int (*read)(void* stream, char* data, int* len))
{
    struct stream_t* s = (struct stream_t*)malloc(sizeof(struct stream_t));
    //TODO NULL
    s->parent = x->stream;
    s->read   = read;
    s->ctx    = stream;
    x->stream = s;
    return x;
}


MO_EXTERN   struct sytx_t*      mo_compile(struct mo_t* m, void* stream, int (*read)(void* stream, char* data, int* len))
{
    struct lex_t* lex = m->lex;
    mo_lex_push(lex, stream, read);
}




MO_EXTERN   void                mo_del(struct mo_t* m);



MO_EXTERN   struct mo_t*        mo_push(struct mo_t* m, struct sytx_t* u);
MO_EXTERN   struct sytx_t*      mo_pop(struct mo_t* m);
