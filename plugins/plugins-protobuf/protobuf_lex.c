#include "protobuf_lex.h"



static void      protobuf_lex_del(void* obj)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)obj;
    //TODO
    free(lex);
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

