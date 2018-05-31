#include "mo.h"


#include <stdlib.h>




struct sytx_t;




struct mo_t
{
    struct lex_t*       lex;
    struct sytx_t*      sytx;
    struct token_t*     token;
    struct result_t*    result;
};




struct sytx_t
{
    struct unit_t*      unit_top;
};




static void             mo_cache_del(void* obj)
{
    struct cache_t* cache = (struct cache_t*)obj;
    if (NULL != cache->stream)
    {
        (cache->stream->del)(cache->stream);
    }

    if (NULL != cache)
    {
        free(cache->cache);
    }

    free(cache);
}

static struct cache_t* mo_cache_new(struct stream_t* stream, int init_cache_size)
{
    char* buf = (char*)malloc(sizeof(char) * init_cache_size);
    if (NULL == buf)
    {
        return NULL;
    }

    struct cache_t* cache = (struct cache_t*)malloc(sizeof(struct cache_t));
    if (NULL == cache)
    {
        free(buf);
        return NULL;
    }

    cache->size         =   sizeof(struct cache_t);
    cache->del          =   mo_cache_del;
    cache->prev         =   NULL;
    cache->stream       =   stream;
    cache->cache_size   =   init_cache_size;
    cache->cache        =   buf;
    cache->pc           =   buf;
    cache->pe           =   buf;
    cache->line         =   buf;
    cache->lino         =   -1;

    return cache;
}

MO_EXTERN   struct mo_t*        mo_new          ()
{
    struct mo_t* mo = (struct mo_t*)malloc(sizeof(struct mo_t));
    mo->lex     =   NULL;
    mo->sytx    =   (struct sytx_t*)malloc(sizeof(struct sytx_t));
    mo->result  =   NULL;
    return mo;
}




MO_EXTERN   void                mo_del          (struct mo_t* mo)
{
    free(mo);
}




MO_EXTERN   void                mo_reg_lex      (struct mo_t* mo, struct lex_t*    x)
{
    mo->lex = x;
}




MO_EXTERN   void                mo_reg_result   (struct mo_t* mo, struct result_t* r)
{
    mo->result = r;
}




MO_EXTERN   void                mo_push_stream  (struct mo_t* mo, struct stream_t* m)
{
    struct param_t* params = mo_get_params(mo);
    struct cache_t* cache  = mo_cache_new(m, params->cache_size);
    cache->prev = mo->lex->cache_top;
    mo->lex->cache_top = cache;
}




MO_EXTERN   void                mo_push_unit    (struct mo_t* mo, struct unit_t*   u)
{
    u->prev = mo->sytx->unit_top;
    mo->sytx->unit_top = u;
}




MO_EXTERN   struct unit_t*      mo_pop_unit     (struct mo_t* mo)
{
    if (NULL == mo->sytx->unit_top)
    {
        return NULL;
    }

    struct unit_t* top = mo->sytx->unit_top;
    mo->sytx->unit_top = mo->sytx->unit_top->prev;
    return top;
}




MO_EXTERN   struct unit_t*      mo_top_unit     (struct mo_t* mo)
{
    return mo->sytx->unit_top;
}




MO_EXTERN   mo_errno            mo_walk(struct mo_t* mo)
{
    mo_action action = MO_ACTION_TRYAGAIN;
    mo_token  token  = MO_TOKEN_ERROR;
    
READ_MORE:
    token = (*(mo->lex->next))(mo->lex, mo->token);
    if (MO_TOKEN_ERROR == token)
    {
        return 111;
    }

TRYAGAIN:
    action = (*(mo->sytx->unit_top->accept))(mo->sytx->unit_top, mo->token);
    if (MO_ACTION_NEEDMORE == action)
    {
        ///!    都没没数据了，还在请求更多token，一定是bug了
        if (MO_TOKEN_EOF == token)
        {
            return 111;
        }
        
        goto READ_MORE;
    }

    if (MO_ACTION_TRYAGAIN == action)
    {
        ///!    所有的数据接收完毕，且刚好识别完毕
        if (MO_TOKEN_EOF == token)
        {
            return 0;
        }
        
        goto TRYAGAIN;
    }
    
    //if (MO_ACTION_ERROR == action)
    {
        //  TODO 遇到accept识别错误
        return 111;
    }
    
    
}



