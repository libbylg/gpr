#include "mo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MO_CACHE_CAP_DEF    (1024)
#define MO_CACHE_RSRV_DEF   (32)

static int                 mo_classes_count = 0;
static struct class_t      mo_classes[100] = {0};

static void                     mo_class_del(void* obj)
{
    //  DO-NOTHING 类是不可删除的
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

MO_EXTERN   void                mo_object_del(void* obj)
{
    if (NULL == obj) {
        return;
    }

    struct object_t* object = (struct object_t*)obj;
    if ((object->typeid >= 0) && (object->typeid < mo_classes_count)) {
        struct class_t* cls = mo_classes + object->typeid;
        cls->del(obj);
    }
}

static      void                mo_result_del(void* obj)
{
    //TODO
}

MO_EXTERN   struct result_t*    mo_result_new()
{
    struct result_t* r = (struct result_t*)malloc(sizeof(struct result_t));
    r->prev = NULL;
    r->typeid = mo_define_class("result_t", );
    r->error = MO_OK;
    r->desc[0] = '\0';
}

MO_EXTERN   mo_bool             mo_result_ok(struct result_t* r)
{
    return (MO_OK == r->error);
}



MO_EXTERN   struct result_t*    mo_result_errorf(struct result_t* r, int error, char* format, ...)
{
    
    va_list va;
    va_start(va, format);
    r = mo_result_verrorf(r, error, format, va);
    va_end(va);
    return r;
}


MO_EXTERN   struct result_t*    mo_result_clear(struct result_t* r)
{
    r->error = MO_OK;
    r->desc[0] = '\0';
}

MO_EXTERN   struct token_t*     mo_result_verrorf(struct result_t* r, int error, char* format, va_list args)
{
    r->error = error;
    _vsnprintf(r->desc, sizeof(r->desc), format, args);
    r->desc[sizeof(r->desc) - 1] = '\0';
    return r;
}


static void                     mo_token_del(void* o)
{

}

MO_EXTERN struct token_t*       mo_token_new()
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
    return k;
}


MO_EXTERN   mo_bool             mo_token_ok(struct token_t* k)
{
    return (MO_TOKEN_ERROR != k->id);
}

MO_EXTERN   struct token_t*     mo_token_clear(struct token_t*  k)
{
    k->id = MO_TOKEN_UNKNOEN;
    k->opts = 0;
    return k;
}

MO_EXTERN   struct token_t*     mo_token_errorf         (struct token_t* k, struct result_t* r, int error, char* format, ...)
{
    va_list args;
    va_start(args, format);
    mo_result_verrorf(r, error, format, args);
    va_end(args);

    k->id = MO_TOKEN_ERROR;
    return k;
}

MO_EXTERN   struct token_t*     mo_token_as(struct token_t* k, int id, int opts, mo_byte* bgn, mo_byte* end)
{
    if (NULL == k) {
        k = mo_token_new();
        if (NULL == k) {
            return NULL;
        }
    }

    k->id = id;
    k->opts = opts;
    if ((NULL != bgn) && (NULL != end)) {
        k->val_str[0] = bgn;
        k->val_str[1] = end;
    }

    return k;
}

static   void                   mo_stream_del(void* m)
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

static   void   mo_lex_del(void* x)
{
    //  TODO
}

MO_EXTERN   struct lex_t*       mo_lex_new(void* ctx, MO_NEXT_CALLBACK   next, int cap)
{
    int size = sizeof(struct lex_t);
    struct lex_t* x = (struct lex_t*)malloc(size);
    if (NULL == x) {
        return NULL;;
    }

    mo_byte* buf = (mo_byte*)malloc(cap + 32 + sizeof(void*));
    if (NULL == buf) {
        free(x);
        return NULL;
    }
    x->prev = x;
    x->typeid = mo_define_class("lex_t", mo_lex_del);
    
    x->ctx = ctx;
    x->next = next;

    x->token  = NULL;
    x->stream = NULL;
    x->anchor = NULL;

    x->cap = cap;
    x->pos = x->buf;
    x->end = x->pos;
    x->limit = x->buf + cap;
    x->end[0] = '\n';
    return x;
}

MO_EXTERN   void                mo_lex_push_stream(struct lex_t* x, struct stream_t* m)
{
    m->prev = x->stream;
    x->stream = m;
    x->anchor = &(x->stream->anchor);
    return;
}


MO_EXTERN   struct stream_t*    mo_lex_pop_stream(struct lex_t* x)
{
    struct stream_t* top = x->stream;
    x->stream = x->stream->prev;
    x->anchor = &(x->stream->anchor);
    return top;
}

MO_EXTERN   struct token_t*     mo_lex_next_token(struct lex_t* x, struct token_t* k, struct result_t* r)
{
    if (NULL == k) {
        k = mo_token_new();
    }
    return x->next(x->ctx, x, k, r);
}

MO_EXTERN   void                mo_lex_push_back(struct lex_t* x, struct token_t* k)
{
    k->prev = x->token;
    x->token = k;
}
