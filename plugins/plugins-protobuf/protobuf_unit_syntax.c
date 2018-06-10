#include "mo.h"
#include "stdlib.h"
#include "plugins/plugins-protobuf/protobuf_token.h"

struct protobuf_unit_syntax_t
{
    struct unit_t   super;
    int             state;
};


static mo_action protobuf_unit_syntax_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_syntax_t* u = (struct protobuf_unit_syntax_t*)n;

    if (MO_TOKEN_EOF == t->token)
    {
        mo_push_result(u->super.mo, mo_result_new("parser", 111, ""));
        return MO_ACTION_ERROR;
    }

    switch (u->state)
    {
    case 0: //  初始状态
        if (MO_TOKEN_syntax != t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
        return MO_ACTION_NEEDMORE;
    case 1: //  等 = 
        if ('=' != t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }
        break;
    case 2: //  等 "xxx"
        if (MO_TOKEN_STRING == t->token)
        {
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
        break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "'syntax' 之后需要 '='"));
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



