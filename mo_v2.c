#include "mo.h"

#include "stdlib.h"

#define MO_CACHE_CAP_DEF    (1024)
#define MO_CACHE_RSRV_DEF   (32)

static int                  mo_objects_count = 0;
static struct object_t      mo_objects[100] = {0};

MO_EXTERN   int                 mo_register_object(char* name, MO_DEL_CALLBACK del)
{
    if (mo_objects_count >= (sizeof(mo_objects) / sizeof(mo_objects[0]))) {
        return -1;
    }

    for (int i = 0; i < mo_objects_count; i++) {
        struct object_t* obj = mo_objects + i;
        if (0 == strcmp(obj->name, name)) {
            return obj->type;
        }
    }

    struct object_t* obj = mo_objects + mo_objects_count;
    obj->type = mo_objects_count;
    strncpy(obj->name, sizeof(obj->name), name);
    obj->del = del;
    return obj->type;
}

MO_EXTERN   struct result_t*    mo_result_clear(struct result_t* r)
{
    r->error = 0;
    r->text[0] = '\0';
    return r;
}

MO_EXTERN   mo_bool             mo_result_ok(struct result_t* r)
{
    return (MO_OK == r->error);
}

MO_EXTERN   struct token_t*     mo_token_clear          (struct token_t*  k);

static   struct cache_t*     mo_cache_new(int cap, int rsrv)
{
    if (cap <= MO_CACHE_CAP_DEF) {
        cap = MO_CACHE_CAP_DEF;
    }

    if (rsrv <= MO_CACHE_RSRV_DEF) {
        rsrv = MO_CACHE_RSRV_DEF;
    }

    int size = sizeof(struct cache_t) + cap + rsrv + 1;
    struct cache_t* c = (struct cache_t*)malloc(size);
    if (NULL == c) {
        return NULL;
    }
    
    c->prev = c;
    c->type = mo_register_object("cache_t", free);
    c->anchor = NULL;
    c->rsrv = rsrv;
    c->cap = cap;
    c->pos = c->buf;
    c->end = c->pos;
    c->limit = c->buf + cap;
    c->end[0] = '\n';
    return c;
}

static   void   mo_lex_del(void* x)
{
    //  TODO
}

MO_EXTERN   struct lex_t*       mo_lex_new(void* ctx, MO_NEXT_CALLBACK   next, int cap, int rsrv)
{
    struct lex_t* x = (struct lex_t*)malloc(sizeof(struct lex_t));
    if (NULL == x) {
        return NULL;;
    }

    x->prev = x;
    x->type = mo_register_object("lex_t", mo_lex_del);
    x->ctx = ctx;
    x->next = next;
    x->cache = mo_cache_new(cap, rsrv);
    x->stream = NULL;
    return x;
}

static void mo_sytx_del(void* y)
{
}

MO_EXTERN   struct sytx_t*      mo_sytx_new()
{
    struct sytx_t* y = (struct sytx_t*)malloc(sizeof(struct sytx_t));
    if (NULL == y) {
        return NULL;;
    }

    y->prev = y;
    y->type = mo_register_object("sytx_t", mo_sytx_del);
    y->unit = NULL;

    return y;
}

MO_EXTERN   void                mo_unit_del(void* u)
{

}

MO_EXTERN   struct unit_t*      mo_unit_new(void* ctx, MO_ACCEPT_CALLBACK accept)
{
    struct unit_t* u = (struct unit_t*)malloc(sizeof(struct unit_t));
    if (NULL == u) {
        return NULL;
    }
    u->prev = u;
    u->type = mo_register_object("unit_t", mo_unit_del);
    u->ctx = ctx;
    u->accept = accept;
    return u;
}

MO_EXTERN   void                mo_stream_del(void* m)
{

}

MO_EXTERN   struct stream_t*    mo_stream_new(void* ctx, MO_READ_CALLBACK   read, MO_READ_CALLBACK close)
{
    struct stream_t* s = (struct stream_t*)malloc(sizeof(struct stream_t));
    if (NULL == s) {
        return NULL;
    }
    s->prev = s;
    s->type = mo_register_object("stream_t", mo_stream_del);
    s->anchor.name = NULL;
    s->anchor.lino = 0;
    s->anchor.line = NULL;
    s->ctx = ctx;
    s->close = close;
    s->read = read;
    return s;
}

static void mo_token_del(void* k)
{
}

MO_EXTERN struct token_t*  mo_token_new()
{
    struct token_t* k = (struct token_t*)malloc(sizeof(struct token_t));
    if (NULL == k) {
        return NULL;
    }

    k->prev = k;
    k->type = mo_register_object("token_t", mo_token_del);
    k->size = sizeof(struct token_t);
    k->id = MO_TOKEN_UNKNOWN;
    k->opts = 0;
    k->text[0] = '\0';
    return k;
}

MO_EXTERN struct result_t*  mo_result_new()
{
    struct result_t* r = (struct result_t*)malloc(sizeof(struct result_t));
    if (NULL == r) {
        return NULL;
    }

    r->prev = r;
    r->type = mo_register_object("result_t", free);
    r->error = MO_OK;
    r->text[0] = '\0';
    return r;
}

static void mo_compile_del(void* p)
{

}

MO_EXTERN   struct compile_t*   mo_compile_init(struct compile_t* p, struct lex_t* x, struct sytx_t* y)
{
    p->prev = p;
    p->type = mo_register_object("compile_t", mo_compile_del);
    p->lex = x;
    p->sytx = y;
    p->token = mo_token_new();
    p->result = mo_result_new();
    return p;
}

MO_EXTERN   void                mo_compile_clear        (struct compile_t* p);

MO_EXTERN   void                mo_lex_push_stream(struct lex_t* x, struct stream_t* m)
{
    m->prev = x->stream;
    x->stream = m;
    x->cache->anchor = &(x->stream->anchor);
    return;
}


MO_EXTERN   struct stream_t*    mo_lex_pop_stream(struct lex_t* x)
{
    struct stream_t* top = x->stream;
    x->stream = x->stream->prev;
    x->cache->anchor = &(x->stream->anchor);
    return top;
}

MO_EXTERN   void                mo_sytx_push_unit(struct sytx_t* y, struct unit_t*   u)
{
    u->prev = y->unit;
    y->unit = u;
    return;
}

MO_EXTERN   struct unit_t*      mo_compile_top_unit(struct sytx_t* y)
{
    struct unit_t* top = y->unit;
    y->unit = y->unit->prev;
    return top;
}

MO_EXTERN   void                mo_sytx_drive_default(void* ctx, struct compile_t* p)
{

}

