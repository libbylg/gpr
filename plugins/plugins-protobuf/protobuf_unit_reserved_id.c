#include "protobuf.h"
#include "protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_reserved_id_t
{
    struct unit_t   super;
    int             state;
};

static mo_action protobuf_unit_reserved_id_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_reserved_id_t* u = (struct protobuf_unit_reserved_id_t*)n;
    switch (u->state)
    {
    case 0: //  123/max/min
        switch (t->token)
        {
        case MO_TOKEN_INTEGER:          //  123
        case MO_TOKEN_NAME:             //  max/min
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  to      可选
        if (MO_TOKEN_to == t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }

        mo_pop_unit(u->super.mo);
        return MO_ACTION_NEEDMORE;
    break;
    case 2:
        switch (t->token)
        {
        case MO_TOKEN_INTEGER:          //  456
        case MO_TOKEN_NAME:             //  max/min
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    }
    
    mo_push_result(u->super.mo, mo_result_new("parser", 111, "unexpected token"));
    return MO_ACTION_ERROR;
}

MO_EXTERN struct unit_t* protobuf_unit_reserved_id_new()
{
    struct protobuf_unit_reserved_id_t* unit = (struct protobuf_unit_reserved_id_t*)malloc(sizeof(struct protobuf_unit_reserved_id_t));
    unit->super.size    =   sizeof(struct protobuf_unit_reserved_id_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_reserved_id_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}




