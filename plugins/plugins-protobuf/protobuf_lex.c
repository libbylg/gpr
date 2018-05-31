#include "protobuf_lex.h"

#define     MO_EXTR_LEN     (32)

static void      protobuf_lex_del(void* obj)
{
    struct protobuf_lex_t* lex = (struct protobuf_lex_t*)obj;
    //TODO
    free(lex);
}

static mo_token  protobuf_lex_next(struct lex_t*    x, struct token_t* t)
{
    //  {}
    //  ()
    //  ;
    //  ,
    //  =
    //  <>
    //  "xxxx"
    //  12345
    //  message
    //  service
    //  required
    //  optional
    //  repeated
    //  rpc
    //  init
    //  exit
    //  reserved
    //      double
    //      float
    //      int32
    //      int64
    //      uint32
    //      uint64
    //      sint32
    //      sint64
    //      fixed32
    //      fixed32
    //      sfixed32
    //      sfixed64
    //      bool
    //      string
    //      bytes
    //  enum
    //  import
    //  extensions
    //  extend
    //  oneof
    //  map
    //  package
    //  option
    //  returns
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

