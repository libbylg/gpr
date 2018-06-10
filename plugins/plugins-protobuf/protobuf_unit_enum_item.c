
#include "protobuf.h"
#include "plugins/plugins-protobuf/protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_enum_item_t
{
    struct unit_t   super;
    int             state;
};

static mo_action protobuf_unit_enum_item_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_enum_item_t* u = (struct protobuf_unit_enum_item_t*)n;
    switch (u->state)
    {
    case 0: //  等  NAME
        if (MO_TOKEN_NAME == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  等 =
        if ('=' == t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 2: //  等 CONST_INTEGER
        if (MO_TOKEN_INTEGER == t->token)    //  可选
        {
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    }
    
    mo_push_result(u->super.mo, mo_result_new("parser", 111, "unexpected token"));
    return MO_ACTION_ERROR;
}

MO_EXTERN struct unit_t* protobuf_unit_enum_item_new()
{
    struct protobuf_unit_enum_item_t* unit = (struct protobuf_unit_enum_item_t*)malloc(sizeof(struct protobuf_unit_enum_item_t));
    unit->super.size    =   sizeof(struct protobuf_unit_enum_item_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_enum_item_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}




