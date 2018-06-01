#ifndef __protobuf_lex_
#define __protobuf_lex_


#include "mo.h"



struct protobuf_lex_t
{
    struct lex_t    base;
};
MO_EXTERN   struct protobuf_lex_t* protobuf_lex_new(int init_cache_size);



enum
{
    CM_DEC      =   0x0001,
    CM_HEX      =   0x0002,
    CM_NEWLINE  =   0X0004,

    CM_ALPHA    =   0x0010,
    CM_ALPHA    =   0x0020,
};
MO_EXTERN short cm[256];
#endif//__protobuf_lex_


