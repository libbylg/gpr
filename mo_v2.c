#include "mo.h"

#define MO_CACHE_CAP_DEF    (1024)
#define MO_CACHE_RSRV_DEF   (32)

//  词法分析对象
struct lex_t
{
    void*               prev;       //  前一个词法对象
    int                 type;
    void*               ctx;        //  词法上下文
    MO_NEXT_CALLBACK    next;       //  词法识别的函数
};


//  语法分析对象
struct sytx_t
{
    void*               prev;       //  前一个词法对象
    int                 type;
    void*               ctx;        //  词法上下文
    MO_DRIVE_CALLBACK   drive;
};


MO_EXTERN   struct lex_t*       mo_lex_new      (void* ctx, MO_NEXT_CALLBACK next)
{
    struct lex_t* x = (struct lex_t*)malloc(sizeof(struct lex_t));
    if (NULL == x) {
        return NULL;;
    }

    x->prev = x;
    x->type = MO_TYPE(lex_t);
    x->ctx = ctx;
    x->next = next;

    return x;
}

MO_EXTERN   struct sytx_t*      mo_sytx_new     (void* ctx, MO_DRIVE_CALLBACK drive)
{
    struct sytx_t* y = (struct sytx_t*)malloc(sizeof(struct sytx_t));
    if (NULL == y) {
        return NULL;;
    }

    y->prev = y;
    y->type = MO_TYPE(sytx_t);
    y->ctx = ctx;
    y->drive = drive;

    return y;
}

MO_EXTERN   struct cache_t*     mo_cache_new(int cap, int rsrv)
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
    c->type = MO_TYPE(cache_t);
    c->anchor = NULL;
    c->rsrv = rsrv;
    c->cap = cap;
    c->pos = c->buf;
    c->end = c->pos;
    c->limit = c->buf + cap;
    c->end[0] = '\n';
    return c;
}

MO_EXTERN   struct unit_t*      mo_unit_new(void* ctx, MO_ACCEPT_CALLBACK accept)
{
    struct unit_t* u = (struct unit_t*)malloc(sizeof(struct unit_t));
    if (NULL == u) {
        return NULL;
    }
    u->prev = u;
    u->type = MO_TYPE(unit_t);
    u->ctx = ctx;
    u->accept = accept;
    return u;
}

MO_EXTERN   struct stream_t*    mo_stream_init(struct stream_t* s, void* ctx, MO_READ_CALLBACK   read, MO_READ_CALLBACK close)
{
    s->prev = s;
    s->type = MO_TYPE(stream_t);
    s->anchor.name = NULL;
    s->anchor.lino = 0;
    s->anchor.line = NULL;
    s->ctx = ctx;
    s->close = close;
    s->read = read;
    return s;
}

static struct token_t*  mo_token_new()
{
    struct token_t* k = (struct token_t*)malloc(sizeof(struct token_t));
    if (NULL == k) {
        return NULL;
    }

    k->prev = k;
    k->type = MO_TYPE(token_t);
    k->size = sizeof(struct token_t);
    k->id = MO_TOKEN_UNKNOWN;
    k->opts = 0;
    k->text[0] = '\0';
    return k;
}

static struct result_t*  mo_result_new()
{
    struct result_t* r = (struct result_t*)malloc(sizeof(struct result_t));
    if (NULL == r) {
        return NULL;
    }

    r->prev = r;
    r->type = MO_TYPE(result_t);
    r->error = MO_OK;
    r->text[0] = '\0';
    return r;
}

MO_EXTERN   struct compile_t*   mo_compile_init(struct compile_t* p, void* ctx, struct sytx_t* y, struct lex_t* x, struct cache_t* c)
{
    if (NULL == c) {
        c = mo_cache_new(0, 0);
        if (NULL == c) {
            return NULL;
        }
    }

    p->prev = p;
    p->type = MO_TYPE(compile_t);
    p->ctx = ctx;
    p->sytx = y;
    p->lex = x;
    p->cache = c;
    p->stream = NULL; //  TODO 考虑定义缺省值
    p->unit = NULL; //  TODO 考虑定义缺省值
    p->token = mo_token_new();
    p->result = mo_result_new();
    return p;
}

MO_EXTERN   void                mo_compile_clear        (struct compile_t* p);

MO_EXTERN   void                mo_compile_push_stream(struct compile_t* p, struct stream_t* s)
{
    s->prev = p->stream;
    p->stream = s;
    p->cache->anchor = &(p->stream->anchor);
    return;
}


MO_EXTERN   struct stream_t*    mo_compile_pop_stream(struct compile_t* p)
{
    struct stream_t* top = p->stream;
    p->stream = p->stream->prev;
    p->cache->anchor = &(p->stream->anchor);
    return top;
}

MO_EXTERN   void                mo_compile_push_unit(struct compile_t* p, struct unit_t*   u)
{
    u->prev = p->unit;
    p->unit = u;
    return;
}

MO_EXTERN   struct unit_t*      mo_compile_top_unit(struct compile_t* p)
{
    struct unit_t* top = p->unit;
    p->unit = p->unit->prev;
    return top;
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

MO_EXTERN   void                mo_sytx_drive_default(void* ctx, struct compile_t* p)
{

}

