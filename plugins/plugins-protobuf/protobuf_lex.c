#include "protobuf_lex.h"

#include "protobuf_token.h"

#include <stdlib.h>


#define     MO_EXTR_LEN     (32)




static void      protobuf_lex_del(void* obj)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)obj;
    //TODO
    free(lex);
}




MO_EXTERN   struct protobuf_lex_t* protobuf_lex_new(int init_cache_size)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)malloc(sizeof(struct protobuf_lex_t));

    lex->super.size       =   sizeof(struct protobuf_lex_t);
    lex->super.cache_top  =   NULL;
    lex->super.del        =   protobuf_lex_del;
    lex->super.next       =   protobuf_lex_next;
    return lex;
}




///!    定位到第一个非注释非空白处
static int       protobuf_lex_locate(struct lex_t*  x)
{
RETRY:
    register struct cache_t*    cache   = x->cache_top;
    register struct stream_t*   stream  = x->cache_top->stream;
    register char*              pc      = x->cache_top->pc;
    register char*              pe      = x->cache_top->pe;

    ///!    跳过所有的空白
    while (cm[*pc] & CM_SPACE)
    {
        pc++;
    }

    ///!    如果遇到换行,需要判断到底是遇到真换行还是遇到的是缓冲区的数据被读取完了
    if (*pc != '\n')
    {
        cache->pc = pc; ///<    先将已经忽略过的空白跳过
        return MO_READ_OK;
    }

    ///!    如果确实遇到了文本换行
    if (pc != pe)
    {
        cache->line++;  ///<    进入下一行
        pc++;           ///<    跳过\n字符
        goto RETRY;
    }

    ///!    如果是缓冲区的数据读取完了
    cache->pc = pc;
    int ret = mo_cache_load(x);
    switch  (ret)
    {
    case MO_READ_OK:    
        goto RETRY;
    case MO_READ_EOF:
        x->cache_top = x->cache_top->prev;      ///<    退栈
        (*(cache->del))(cache);    
        goto RETRY;
    case MO_READ_ERROR: 
    default:
        mo_push_result(x->mo, mo_result_new("lex", 111, "load data failed"));
        return ret;
    }
}




static mo_token  protobuf_lex_setup_token(struct lex_t*    x, struct token_t* t, int size, mo_token token)
{
    t->token = token;
    x->cache_top->pc += size;
    return token;
}




static mo_token  protobuf_lex_next(struct lex_t*    x, struct token_t* t)
{
RETRY:
    ///!    已经没有任何数据可以读取了
    if (NULL == x->cache_top)
    {
        return MO_TOKEN_EOF;
    }

    int ret = protobuf_lex_locate(x->cache_top);
    if (MO_READ_OK != ret)
    {
        if (MO_READ_ERROR == ret)
        {
            mo_push_result(x->mo, mo_result_new("lex", 111, "locate failed"));
            return MO_TOKEN_ERROR;
        }

        if (MO_READ_EOF == ret)
        {
            return MO_TOKEN_EOF;
        }
    }


    register char* pc = x->cache_top->pc;
    switch (*pc)
    {
    case '{':  case '}':  case  '(':  case ')':  case  ';':
    case ',':  case '=':  case  '<':  case '>':  
        return protobuf_lex_setup_token(x, t, 1, *pc);   
    case 'e':   //  enum    exit    extensions  extend
        if ( ('n' == pc[1])  
        &&   ('u' == pc[2])  
        &&   ('m' == pc[3]) 
        &&   (0 == (cm[pc[4]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 4, MO_TOKEN_enum);
        }

        if ( ('x' == pc[1])  
        &&   ('i' == pc[2])  
        &&   ('t' == pc[3]) 
        &&   (0 == (cm[pc[4]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 4,  MO_TOKEN_exit);
        }

        if ( ('x' == pc[1])  
        &&   ('t' == pc[2])  
        &&   ('e' == pc[3]) 
        &&   ('n' == pc[4]) 
        &&   ('d' == pc[5]) 
        &&   (0 == (cm[pc[6]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 6,  MO_TOKEN_extend);
        }

        if ( ('x' == pc[1])  
        &&   ('t' == pc[2])  
        &&   ('e' == pc[3]) 
        &&   ('n' == pc[4]) 
        &&   ('s' == pc[5]) 
        &&   ('i' == pc[6]) 
        &&   ('o' == pc[7]) 
        &&   ('n' == pc[8]) 
        &&   ('s' == pc[9]) 
        &&   (0 == (cm[pc[10]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 10,  MO_TOKEN_extensions);
        }

        return protobuf_try_name(x->cache_top, t, 1);
    case 'i':   //  init        import
        if ( ('n' == pc[1])  
        &&   ('i' == pc[2])  
        &&   ('t' == pc[3]) 
        &&   (0 == (cm[pc[4]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 4,  MO_TOKEN_init);
        }

        if ( ('m' == pc[1])  
        &&   ('p' == pc[2])  
        &&   ('o' == pc[3]) 
        &&   ('r' == pc[4]) 
        &&   ('t' == pc[5]) 
        &&   (0 == (cm[pc[6]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 6,  MO_TOKEN_import);
        }

        return protobuf_try_name(x->cache_top, t, 1);
    case 'm':   //  map     message
        if ( ('a' == pc[1])  
        &&   ('p' == pc[2])  
        &&   (0 == (cm[pc[3]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 3,  MO_TOKEN_map);
        }

        if ( ('e' == pc[1])  
        &&   ('s' == pc[2])  
        &&   ('s' == pc[3]) 
        &&   ('a' == pc[4]) 
        &&   ('g' == pc[5]) 
        &&   ('e' == pc[6]) 
        &&   (0 == (cm[pc[7]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 7,  MO_TOKEN_message);
        }

            return protobuf_try_name(x->cache_top, t, 1);
    case 'o':   //  oneof   optional    option
        if ( ('n' == pc[1])  
        &&   ('e' == pc[2])  
        &&   ('o' == pc[3])  
        &&   ('f' == pc[4])  
        &&   (0 == (cm[pc[5]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 5,  MO_TOKEN_oneof);
        }

        if ( ('p' == pc[1])  
        &&   ('t' == pc[2])  
        &&   ('i' == pc[3]) 
        &&   ('o' == pc[4]) 
        &&   ('n' == pc[5]) 
        &&   ('a' == pc[6]) 
        &&   ('l' == pc[7]) 
        &&   (0 == (cm[pc[8]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 8,  MO_TOKEN_optional);
        }

        if ( ('p' == pc[1])  
        &&   ('t' == pc[2])  
        &&   ('i' == pc[3]) 
        &&   ('o' == pc[4]) 
        &&   ('n' == pc[5]) 
        &&   (0 == (cm[pc[6]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 6,  MO_TOKEN_option);
        }

            return protobuf_try_name(x->cache_top, t, 1);
    case 'p':   //  package
        if ( ('a' == pc[1])  
        &&   ('c' == pc[2])  
        &&   ('k' == pc[3]) 
        &&   ('a' == pc[4]) 
        &&   ('g' == pc[5]) 
        &&   ('e' == pc[6]) 
        &&   (0 == (cm[pc[7]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 7,  MO_TOKEN_package);
        }

            return protobuf_try_name(x->cache_top, t, 1);
    case 'r':   //  repeated    required    reserved    returns rpc
        if ( ('e' == pc[1])  
        &&   ('p' == pc[2])  
        &&   ('e' == pc[3]) 
        &&   ('a' == pc[4]) 
        &&   ('t' == pc[5]) 
        &&   ('e' == pc[6]) 
        &&   ('d' == pc[7]) 
        &&   (0 == (cm[pc[8]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 8,  MO_TOKEN_repeated);
        }

        if ( ('e' == pc[1])  
        &&   ('q' == pc[2])  
        &&   ('u' == pc[3]) 
        &&   ('i' == pc[4]) 
        &&   ('r' == pc[5]) 
        &&   ('e' == pc[6]) 
        &&   ('d' == pc[7]) 
        &&   (0 == (cm[pc[8]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 8,  MO_TOKEN_required);
        }

        if ( ('e' == pc[1])  
        &&   ('s' == pc[2])  
        &&   ('e' == pc[3]) 
        &&   ('r' == pc[4]) 
        &&   ('v' == pc[5]) 
        &&   ('e' == pc[6]) 
        &&   ('d' == pc[7]) 
        &&   (0 == (cm[pc[8]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 8,  MO_TOKEN_reserved);
        }

        if ( ('e' == pc[1])  
        &&   ('t' == pc[2])  
        &&   ('u' == pc[3]) 
        &&   ('r' == pc[4]) 
        &&   ('n' == pc[5]) 
        &&   ('s' == pc[6]) 
        &&   (0 == (cm[pc[7]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 7,  MO_TOKEN_returns);
        }

        if ( ('p' == pc[1])  
        &&   ('c' == pc[2])  
        &&   (0 == (cm[pc[3]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 3,  MO_TOKEN_rpc);
        }

            return protobuf_try_name(x->cache_top, t, 1);
    case 's':   //  service
        if ( ('e' == pc[1])  
        &&   ('r' == pc[2])  
        &&   ('v' == pc[3])
        &&   ('i' == pc[4])  
        &&   ('c' == pc[5])
        &&   ('e' == pc[6])  
        &&   (0 == (cm[pc[7]]&(CM_ALPHA|CM_DEC))) )
        {
            return protobuf_lex_setup_token(x, t, 7,  MO_TOKEN_service);
        }

            return protobuf_try_name(x->cache_top, t, 1);
    case 'a':  case 'b':  case 'c':  case 'd':/*case 'e':*/
    case 'f':  case 'g':  case 'h':  case 'j':/*case 'i':*/
    case 'k':  case 'l':/*case 'm':*/case 'n':/*case 'o':*/
  /*case 'p':*/case 'q':/*case 'r':  case 's':*/case 't':  
    case 'u':  case 'v':  case 'w':  case 'x':  case 'y':  
    case 'z':
    case 'A':  case 'B':  case 'C':  case 'D':  case 'E':  
    case 'F':  case 'G':  case 'H':  case 'J':  case 'I':  
    case 'K':  case 'L':  case 'M':  case 'N':  case 'O':  
    case 'P':  case 'Q':  case 'R':  case 'S':  case 'T':  
    case 'U':  case 'V':  case 'W':  case 'X':  case 'Y':  
    case 'Z':
    case '_':
        return protobuf_try_name(x, t, 1);
    case '0':  case '1':  case '2':  case '3':  case '4':  
    case '5':  case '6':  case '7':  case '8':  case '9':
        return protobuf_try_number(x, t, 1);
    case '"':
        return protobuf_try_string(x, t, 1);
    case '/':
        if ('*' == pc[1])  
        {
            if (0 != protobuf_try_comment_range(x, t, 2))
            {
                mo_push_result(x->mo, mo_result_new("lex", 111, "invalid range comment"));
                return MO_TOKEN_ERROR;
            }

            goto RETRY;
        }

        if ('/' == pc[1])
        {
            protobuf_try_comment_line(x, t, 2);
            goto RETRY; 
        }
        
        //mo_push_result(mo_result_new("lex", 111, "invalid char for lex '0x%.2D'", (unsigned int)(*pc)));
        //return MO_TOKEN_ERROR;
    default:
        mo_push_result(x->mo, mo_result_new("lex", 111, "invalid char for lex '0x%.2D'", (unsigned int)(*pc)));
        return MO_TOKEN_ERROR;
    }
}




static mo_token  protobuf_try_name      (struct cache_t* cache, struct token_t* t, int prefix_len)
{
RETRY:
    register char* pc = cache->pc + prefix_len;
    while (cm[*pc] & (CM_ALPHA | CM_DEC))
    {
        pc++;
    }

    //  如果遇到换行符,需要检查一下是否是真的换行符号
    if (*pc == '\n')
    {
        //  遇到的并不是真的换行符号,那么可能是缓冲区不够,此时可以加载一次缓冲区数据
        if (pc == cache->pe)
        {
            prefix_len = pc - cache->pc;
            int ret = mo_cache_load(cache);
            if (MO_READ_OK == ret)
            {
                goto RETRY;
            }

            if (MO_READ_ERROR == ret)
            {
                mo_push_result(cache->mo, mo_result_new("lex", 111, "load cache failed"));
                return MO_TOKEN_ERROR;
            }

            //  MO_READ_EOF 这种情况直接结束当前处理
        }
    }

    t->token   = MO_TOKEN_NAME;
    t->text[0] = cache->pc;
    t->text[1] = pc;

    return MO_TOKEN_NAME;
}




static mo_token  protobuf_try_number    (struct cache_t* cache, struct token_t* t, int prefix_len)
{

}




static mo_token  protobuf_try_string    (struct cache_t* cache, struct token_t* t, int prefix_len)
{
    RETRY:
    char* pc = cache->pc + prefix_len;
    while (0 != (cm[*pc] & CM_STRFLAG))
    {
        pc++;
    }

    if ('\"' == *pc)
    {
        t->token   = MO_TOKEN_STRING;
        t->text[0] = cache->pc;
        t->text[1] = pc;
        pc++;
        return MO_TOKEN_STRING;
    }

    if (*pc == '\\')
    {
        pc++;
        switch (*pc)
        {
        case '\\':
        case '\"':
        case 't':
        case 'v':
        case 'r':
        case 'n':
        case 'a':
        case 'b':
        case 'x':
        case 'u':
        case '\n':
            if (pc == cache->pe)
            {
                prefix_len = ((pc - cache->pc) - 1);    ///<    由于碰到转义字符,所以实际识别出来的数据长度应该少一个字符
                int ret = mo_cache_load(cache);
                if (MO_READ_OK == ret)
                {
                    pc++;
                    goto RETRY; //  TODO bug
                }

                if (MO_READ_ERROR == ret)
                {
                    mo_push_result(cache->mo, mo_result_new("lex", 111, "load cache failed"));
                    return MO_TOKEN_ERROR;
                }

                if (MO_READ_EOF == ret)
                {
                    mo_push_result(cache->mo, mo_result_new("lex", 111, "string do not end correctly"));
                    return MO_TOKEN_ERROR;
                }
            }

            cache->lino++;
            cache->line = pc + 1;
            pc++;
            goto RETRY;
        default:
            mo_push_result(cache->mo, mo_result_new("lex", 111, "unrecogenized espace char"));
            return MO_TOKEN_ERROR;
        }
    }

    if (*pc == '\n')
    {
        //  遇到的并不是真的换行符号
        if (pc == cache->pe)
        {
            int ret = mo_cache_load(cache);
            if (MO_READ_OK == ret)
            {
                goto RETRY;
            }

            if (MO_READ_ERROR == ret)
            {
                mo_push_result(cache->mo, mo_result_new("lex", 111, "load cache failed"));
                return MO_TOKEN_ERROR;
            }

            //  MO_READ_EOF 这种情况直接结束当前处理

            mo_push_result(cache->mo, mo_result_new("lex", 111, "string do not end correctly"));
            return MO_TOKEN_ERROR;
        }

        //  如果遇到的就是真换行符号
        mo_push_result(cache->mo, mo_result_new("lex", 111, "string do not end correctly"));
        return MO_TOKEN_ERROR;
    }

    //  程序不应该走到这里
    mo_push_result(cache->mo, mo_result_new("lex", 111, "inner-error"));
    return MO_TOKEN_ERROR;
}




static mo_token  protobuf_try_comment_range   (struct lex_t*    x, struct token_t* t, int prefix_len)
{

}




static mo_token  protobuf_try_comment_line   (struct lex_t*    x, struct token_t* t, int prefix_len)
{

}



