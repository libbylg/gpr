#include "mo.h"

#include "stdlib.h"
#include "string.h"

#define MO_CACHE_CAP_DEF    (1024)
#define MO_CACHE_RSRV_DEF   (32)

static int                 mo_classes_count = 0;
static struct class_t      mo_classes[100] = {0};

static void mo_class_del(void* obj)
{
}

MO_EXTERN   int                 mo_define_class(char* name, MO_DEL_CALLBACK del)
{
    if (0 == mo_classes_count) {
        struct class_t* cls = mo_classes + 0;
        cls->prev = cls;
        cls->typeid = 0;    //  指向自己
        strncpy(cls->name, "class_t", sizeof(cls->name));
        cls->del = mo_class_del;
        mo_classes_count = 1;
    }

    if (mo_classes_count >= (sizeof(mo_classes) / sizeof(mo_classes[0]))) {
        return -1;
    }

    for (int i = 0; i < mo_classes_count; i++) {
        struct class_t* cls = mo_classes + i;
        if (0 == strcmp(cls->name, name)) {
            return cls->typeid;
        }
    }

    struct class_t* cls = mo_classes + mo_classes_count;
    cls->typeid = mo_classes_count;
    strncpy(cls->name, name, sizeof(cls->name));
    cls->del = del;
    return cls->typeid;
}

MO_EXTERN   struct class_t*     mo_class_of(int typeid)
{
    if ((typeid < 0) || (typeid > mo_classes_count)) {
        return NULL;
    }

    return mo_classes + typeid;
}

MO_EXTERN   int                 mo_typeid_of(char* name)
{
    for (int i = 0; i < mo_classes_count; i++) {
        struct class_t* cls = mo_classes + i;
        if (0 == strcmp(cls->name, name)) {
            return cls->typeid;
        }
    }

    return -1;
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
    c->typeid = mo_define_class("cache_t", free);
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
    int size = sizeof(struct lex_t) + cap + rsrv + 1;
    struct lex_t* x = (struct lex_t*)malloc(size);
    if (NULL == x) {
        return NULL;;
    }

    mo_byte* buf = (mo_byte*)malloc(size);
    if (NULL == buf) {
        return NULL;
    }
    x->prev = x;
    x->typeid = mo_define_class("lex_t", mo_lex_del);
    x->ctx = ctx;
    x->next = next;
    x->stream = NULL;
    
    x->anchor = NULL;
    x->rsrv = rsrv;
    x->cap = cap;
    x->pos = c->buf;
    x->end = c->pos;
    x->limit = c->buf + cap;
    x->end[0] = '\n';
    return x;
}

MO_EXTERN   mo_token    mo_lex_next_token(struct lex_t* x, struct token_t* t, struct result_t* r)
{
    return x->next(x->ctx, x, t, r);
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
    y->typeid = mo_define_class("sytx_t", mo_sytx_del);
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
    u->typeid = mo_define_class("unit_t", mo_unit_del);
    u->ctx = ctx;
    u->accept = accept;
    return u;
}

MO_EXTERN   void                mo_stream_del(void* m)
{

}

MO_EXTERN   struct stream_t*    mo_stream_new(void* ctx, MO_READ_CALLBACK   read, MO_CLOSE_CALLBACK close)
{
    struct stream_t* s = (struct stream_t*)malloc(sizeof(struct stream_t));
    if (NULL == s) {
        return NULL;
    }
    s->prev = s;
    s->typeid = mo_define_class("stream_t", mo_stream_del);
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
    k->typeid = mo_define_class("token_t", mo_token_del);
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
    r->typeid = mo_define_class("result_t", free);
    r->error = MO_OK;
    r->text[0] = '\0';
    return r;
}

MO_EXTERN   struct result_t*    mo_result_errorf(struct result_t* r, int error, char* format, ...)
{
    r->error = error;
    va_list va;
    va_start(va, format);
    _vsnprintf(r->text, format, va);
    va_end(va);

    return r;
}

static void mo_compile_del(void* p)
{

}

MO_EXTERN   struct compile_t*   mo_compile_init(struct compile_t* p, struct lex_t* x, struct sytx_t* y)
{
    p->prev = p;
    p->typeid = mo_define_class("compile_t", mo_compile_del);
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

