#include "mo.h"


#include <stdlib.h>
#include <stdarg.h>



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
        free(cache->buf);
    }

    free(cache);
}

static struct cache_t* mo_cache_new(struct stream_t* stream, int init_buf_size)
{
    int real_size = init_buf_size + MO_TOKEN_LIMIT + 1;
    char* buf = (char*)malloc(sizeof(char) * (real_size));
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
    cache->buf_size     =   init_buf_size;
    cache->buf          =   buf;
    cache->buf_limit    =   cache->buf + init_buf_size;
    cache->pc           =   buf;
    cache->pe           =   buf;
    cache->line         =   buf;
    cache->lino         =   -1;

    ///!    填充几个守卫的值
    *(cache->pe)                =   '\n';
    *(cache->buf_limit)         =   '\n';
    cache->buf[real_size - 1]   =   '\n';

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
    struct params_t* params = mo_get_params(mo);
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




MO_EXTERN int mo_cache_load(struct cache_t*    cache)
{
    struct stream_t* stream = cache->stream;

    ///!    如果缓冲区中还有数据，那么需要将数据移动到buf首部
    int len = (cache->pe - cache->pc);
    //if ((len > 0) && (len <= MO_TOKEN_LIMIT)) ///<    如果token本身比较短，那么可以考虑做一次数据迁移
    if (len > 0)
    {
        memmove(cache->buf, cache->pc, len);
        cache->line -= (cache->pc - cache->buf);
        cache->pc = cache->buf;
        cache->pe = cache->pc + len;
    }

    ///!    开始从文件读取数据
    int ret = (*(stream->read))(stream, &(cache->pe), cache->buf_limit);
    if (MO_READ_OK != ret)
    {
        return ret;
    }

    *(cache->pe) = '\n';
    return MO_READ_OK;
}




MO_EXTERN   void   mo_push_result  (struct mo_t* mo, struct result_t* r)
{
    r->prev    = mo->result;
    mo->result = r->prev;
}




MO_EXTERN   struct result_t*    mo_result_new   (char* module, int value, char* format, ...)
{
    struct result_t* r = (struct result_t*)malloc(sizeof(struct result_t));

    ///!    如果是成功
    if (0 == value)
    {
        r->value     = 0;
        r->module[0] = '\0';
        r->text[0]   = '\0';
        return r;
    }


    module = (NULL == module)?"":module;
    format = (NULL == format)?"":format;


    ///!    如果是失败
    r->value = value;
    strncpy(r->module, module, sizeof(r->module));
    r->module[sizeof(r->module) - 1] = '\0';


    ///!    格式化输出
    va_list valist;
    va_start(valist, format);
    vsnprintf(r->text, sizeof(r->text), format, valist);
    va_end(valist);
    r->text[sizeof(r->text) - 1] = '\0';


    return r;
}




MO_EXTERN   void   mo_clear_result  (struct mo_t* mo)
{
    for (struct result_t* r = mo->result; NULL != r; r = mo->result)
    {
        free(r);
    }
}