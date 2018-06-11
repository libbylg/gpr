#include "protobuf.h"

#include "protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_syntax_t
{
    struct unit_t   super;
    int             state;
};


static mo_action protobuf_unit_syntax_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_syntax_t* u = (struct protobuf_unit_syntax_t*)n;
    switch (u->state)
    {
    case 0: //  synta
        if (MO_TOKEN_syntax == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  = 
        if ('=' == t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 2: //  "xxx"
        if (MO_TOKEN_STRING == t->token)
        {
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "%s failed: state=%d, token=%d", __FUNCTION__, u->state, t->token));
    return MO_ACTION_ERROR;
}


MO_EXTERN struct unit_t* protobuf_unit_syntax_new()
{
    struct protobuf_unit_syntax_t* unit = (struct protobuf_unit_syntax_t*)malloc(sizeof(struct protobuf_unit_syntax_t));
    unit->super.size    =   sizeof(struct protobuf_unit_syntax_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_syntax_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}



