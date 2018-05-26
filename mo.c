#include "mo.h"


#include <stdlib.h>

struct stream_t
{
    void*               ctx;
    MO_READ_CALLBACK    read;

    struct stream_t*    parent;
};

struct lex_t
{
    void*               ctx;
    MO_NEXT_CALLBACK    next;

    struct stream_t*    stream_top;
    struct token_t*     token;
};

struct unit_t
{
    void*               ctx;
    MO_ACCEPT_CALLBACK  accept;

    struct lex_t*       parent;
};

struct sytx_t
{
    struct unit_t*      unit_top;
};

MO_EXTERN   struct unit_t*      mo_unit_new     (void* ctx, MO_ACCEPT_CALLBACK accept)
{
    struct unit_t* u = (struct unit_t*)malloc(sizeof(struct unit_t));
    u->ctx        = ctx;
    u->accept     = accept;
    u->parent     = NULL;
    return u;
}

MO_EXTERN   void                mo_unit_del     (struct unit_t* u)
{
    if (NULL == u)
    {
        return;
    }

    free(u);
    return;
}

MO_EXTERN   struct stream_t*    mo_stream_new   (void* ctx, MO_READ_CALLBACK   read)
{
    struct stream_t* m = (struct stream_t*)malloc(sizeof(struct stream_t));
    m->ctx        = ctx;
    m->read       = read;
    m->parent     = NULL;
    return m;
}

MO_EXTERN   void                mo_stream_del   (struct stream_t* m)
{
    if (NULL == m)
    {
        return;
    }

    free(m);
    return;
}

MO_EXTERN   struct lex_t*       mo_lex_new      (void* ctx, MO_NEXT_CALLBACK   next)
{
    struct lex_t* x = (struct lex_t*)malloc(sizeof(struct stream_t));
    x->ctx        = ctx;
    x->next       = next;
    x->stream_top = NULL;
    return x;
}

MO_EXTERN   void                mo_lex_del      (struct lex_t* x)
{
    if (NULL == x)
    {
        return;
    }

    free(x);
    return;
}

MO_EXTERN   void                mo_lex_put      (struct lex_t* x, struct stream_t* m)
{
    m->parent     = x->stream_top;
    x->stream_top = m;
}

MO_EXTERN   struct sytx_t*      mo_sytx_new     ()
{
    struct sytx_t* y = (struct sytx_t*)malloc(sizeof(struct sytx_t));
    y->unit_top = NULL;
    return y;
}

MO_EXTERN   void                mo_sytx_del     (struct sytx_t* y)
{
    if (NULL == y)
    {
        return;
    }

    free(y);
}

MO_EXTERN   void                mo_sytx_push    (struct sytx_t* y, struct unit_t* u)
{
    u->parent = y->unit_top;
    y->unit_top = u;
}

MO_EXTERN   struct unit_t*      mo_sytx_pop     (struct sytx_t* y)
{
    struct unit_t* u = y->unit_top;
    if (NULL != y->unit_top)
    {
        y->unit_top = y->unit_top->parent;
    }

    return u;
}

MO_EXTERN   struct unit_t*      mo_sytx_top     (struct sytx_t* y)
{
    return y->unit_top;
}

MO_EXTERN   mo_errno            mo_walk(struct sytx_t* y, struct lex_t* x)
{
    while (1)
    {
        mo_token id = (*(x->next))(x->ctx, x, x->token);
        if (MO_TOKEN_ERROR == id)
        {
            return 111;
        }

        mo_action action = (*(y->unit_top->accept))(y->unit_top->ctx, y, x->token);
        if (MO_ACTION_NEEDMORE == action)
        {
            continue;
        }

        if (MO_ACTION_TRYAGAIN == action)
        {
            if (MO_TOKEN_EOF == id)
            {
                return 0;
            }

            goto RETRY;   
        }
    }
}



