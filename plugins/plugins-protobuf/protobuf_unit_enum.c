#include "protobuf_unit.h"

#include "protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_enum_t
{
    struct unit_t   super;
    int             state;
};


static mo_action protobuf_unit_enum_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_enum_t* u = (struct protobuf_unit_enum_t*)n;

    switch (u->state)
    {
    case 0: //  enum
        if (MO_TOKEN_enum == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  NAME
        if (MO_TOKEN_NAME == t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 2: //  {
        if ('{' == t->token)
        {
            u->state = 3;
            mo_push_unit(u->super.mo, protobuf_unit_enum_item_list_new());
            return MO_ACTION_RETRY;
        }
    break;
    case 3: //  }  
        if ('}' == t->token)
        {
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "%s failed: state=%d, token=%d", __FUNCTION__, u->state, t->token));
    return MO_ACTION_ERROR;
}


MO_EXTERN struct unit_t* protobuf_unit_enum_new()
{
    struct protobuf_unit_enum_t* unit = (struct protobuf_unit_enum_t*)malloc(sizeof(struct protobuf_unit_enum_t));
    unit->super.size    =   sizeof(struct protobuf_unit_enum_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_enum_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}



