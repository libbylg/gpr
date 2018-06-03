#include "protobuf_lex.h"

#define     MO_EXTR_LEN     (32)

static void      protobuf_lex_del(void* obj)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)obj;
    //TODO
    free(lex);
}


static int       protobuf_lex_load(struct lex_t*    x)
{
    struct cache_t*  cache  = x->cache_top;
    struct stream_t* stream = x->cache_top->stream;

    // char* pc = cache->pc;
    // char* pe = cache->pe;

    ///!    如果token本身比较短，那么可以考虑做一次数据迁移
    int len = (cache->pe - cache->pc);
    if ((len > 0) && (len <= MO_TOKEN_LIMIT))
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

///!    定位到第一个非注释非空白处
static int       protobuf_lex_locate(struct lex_t*    x)
{
    if (NULL == x->cache_top)
    {
        return MO_READ_ERROR;
    }

    struct cache_t*  cache  = x->cache_top;
    struct stream_t* stream = x->cache_top->stream;

    register char* pc = cache->pc;
    register char* pe = cache->pe;

RETRY:
    ///!    跳过所有的空白
    while (mc[*pc] & CM_SPACE)
    {
        pc++;
    }

    if (*pc == '\n')
    {
        ///!    如果遇到换行,需要判断到底是遇到真换行还是遇到的是缓冲区的数据被读取完了
        ///!    如果是缓冲区的数据读取完了
        if (pc == pe)
        {
            cache->pc = pc;
            int ret = protobuf_lex_load(x);
            if (MO_READ_OK != ret)
            {
                return ret;
            }
        }
        else
        {
            ///!    遇到了真的换行符号
            cache->line++;  ///<    进入下一行
            pc++;           ///<    跳过\n字符
        }

        goto RETRY;
    }

    cache->pc = pc; ///<    先将已经忽略过的空白跳过
    return MO_READ_OK;
}

static mo_token  protobuf_lex_next(struct lex_t*    x, struct token_t* t)
{


}

MO_EXTERN   struct protobuf_lex_t* protobuf_lex_new(int init_cache_size)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)malloc(sizeof(struct protobuf_lex_t));

    lex->size       =   sizeof(struct protobuf_lex_t);
    lex->stream_top =   NULL;
    lex->del        =   protobuf_lex_del;
    lex->next       =   protobuf_lex_next;
    return lex;
}

