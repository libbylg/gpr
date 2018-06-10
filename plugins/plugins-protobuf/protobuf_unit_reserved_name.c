#include "protobuf.h"
#include "protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_reserved_name_t
{
    struct unit_t   super;
    int             state;
};

static mo_action protobuf_unit_reserved_name_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_reserved_name_t* u = (struct protobuf_unit_reserved_name_t*)n;
    switch (u->state)
    {
    case 0: //  等 "xxx"
        if (MO_TOKEN_STRING == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  等 ,
        if (',' == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }

        u->state = 1;
        return MO_ACTION_RETRY;
    break;
    }
    
    mo_push_result(u->super.mo, mo_result_new("parser", 111, "unexpected token"));
    return MO_ACTION_ERROR;
}

MO_EXTERN struct unit_t* protobuf_unit_reserved_name_new()
{
    struct protobuf_unit_reserved_name_t* unit = (struct protobuf_unit_reserved_name_t*)malloc(sizeof(struct protobuf_unit_reserved_name_t));
    unit->super.size    =   sizeof(struct protobuf_unit_reserved_name_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_reserved_name_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}




