#include "mo.h"


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>



///!    cache 管理
#define compile_tOKEN_LIMIT  (32)


///!    词法分析对象
struct lex_t
{
    void*               prev;       //  前一个词法对象
    void*               ctx;        //  词法上下文
    MO_NEXT_CALLBACK    next;       //  词法识别的函数
};



struct sytx_t
{
    void*               prev;   //  前一个词法对象
    MO_DRIVE_CALLBACK   drive;
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
    ///!    + compile_tOKEN_LIMIT 是为了给提前扫描的符号预留预读字符数,避免访问无效内存
    ///!    + 1              是为了当遇到最后一行时,需要额外给缓冲区补一个\n符号
    int real_size = init_buf_size + compile_tOKEN_LIMIT + 1;
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




MO_EXTERN   struct compile_t*        mo_new          ()
{
    struct compile_t* mo = (struct compile_t*)malloc(sizeof(struct compile_t));
    mo->lex     =   NULL;
    mo->sytx    =   (struct sytx_t*)malloc(sizeof(struct sytx_t));
    memset(mo->sytx, 0, sizeof(struct sytx_t));
    mo->result  =   (struct result_t*)malloc(sizeof(struct result_t));
    memset(mo->result, 0, sizeof(struct result_t));
    return mo;
}




MO_EXTERN   void                mo_del          (struct compile_t* mo)
{
    free(mo);
}




MO_EXTERN   void                mo_reg_lex      (struct compile_t* mo, struct lex_t*    x)
{
    mo->lex = x;
}




MO_EXTERN   void                mo_push_stream  (struct compile_t* mo, struct stream_t* m)
{
    struct params_t* params = mo_get_params(mo);
    struct cache_t* cache  = mo_cache_new(m, params->cache_size);
    cache->prev = mo->lex->cache_top;
    mo->lex->cache_top = cache;
}




MO_EXTERN   void                mo_push_unit    (struct compile_t* mo, struct unit_t*   u)
{
    u->prev = mo->sytx->unit_top;
    mo->sytx->unit_top = u;
}




MO_EXTERN   struct unit_t*      mo_pop_unit     (struct compile_t* mo)
{
    if (NULL == mo->sytx->unit_top)
    {
        return NULL;
    }

    struct unit_t* top = mo->sytx->unit_top;
    mo->sytx->unit_top = mo->sytx->unit_top->prev;
    return top;
}




MO_EXTERN   struct unit_t*      compile_top_unit     (struct compile_t* mo)
{
    return mo->sytx->unit_top;
}




MO_EXTERN   mo_errno            mo_walk(struct compile_t* mo)
{
    mo_action action = MO_ACTION_RETRY;
    compile_token  token  = compile_tOKEN_ERROR;
    
READ_MORE:
    token = (*(mo->lex->next))(mo->lex, mo->token);
    if (compile_tOKEN_ERROR == token)
    {
        return 111;
    }

TRYAGAIN:
    action = (*(mo->sytx->unit_top->accept))(mo->sytx->unit_top, mo->token);
    if (MO_ACTION_NEEDMORE == action)
    {
        ///!    都没没数据了，还在请求更多token，一定是bug了
        if (compile_tOKEN_EOF == token)
        {
            return 111;
        }
        
        goto READ_MORE;
    }

    if (MO_ACTION_RETRY == action)
    {
        ///!    所有的数据接收完毕，且刚好识别完毕
        if (compile_tOKEN_EOF == token)
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
    //if ((len > 0) && (len <= compile_tOKEN_LIMIT)) ///<    如果token本身比较短，那么可以考虑做一次数据迁移
    if (len > 0)
    {
        memmove(cache->buf, cache->pc, len);
        cache->line -= (cache->pc - cache->buf);
        cache->pc = cache->buf;
        cache->pe = cache->pc + len;
    }

    ///!    开始从文件读取数据
    int ret = (*(stream->read))(stream, &(cache->pe), cache->buf_limit);
    if (MO_READ_OK == ret)
    {
        *(cache->pe) = '\n';
        return MO_READ_OK;
    }

    if (MO_READ_EOF == ret)
    {
        ///!    当输入文件最后一行不是空行时,需要为文件额外补充一个换行符号
        if ((cache->pe > cache->buf) && (cache->pe[-1] != '\n'))
        {
            cache->pe[0] = '\n';
            cache->pe[1] = '\n';
            cache->pe++;
        }
        return MO_READ_EOF;
    }

    return MO_READ_ERROR;
}




MO_EXTERN   void   mo_push_result  (struct compile_t* mo, struct result_t* r)
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




MO_EXTERN   void   mo_clear_result  (struct compile_t* mo)
{
    for (struct result_t* r = mo->result; NULL != r; r = mo->result)
    {
        free(r);
    }
}



MO_EXTERN   struct params_t*    mo_get_params   (struct compile_t* mo)
{
    return NULL;
}




