#include "mo.h"
#include <stdlib.h>
#include "plugins/plugins-protobuf/protobuf_token.h"

struct protobuf_unit_reserved_t
{
    struct unit_t   super;
    int             state;
};

static mo_action protobuf_unit_reserved_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_reserved_t* u = (struct protobuf_unit_reserved_t*)n;

    switch (u->state)
    {
    case 0: //  等 reserved
        if (MO_TOKEN_option == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1: //  等 = 
        switch (t->token)
        {
        case MO_TOKEN_STRING:
            mo_push_unit(u->super.mo, protobuf_unit_reserved_name_new());
            u->state = 2;
            return MO_ACTION_RETRY;
        break;
        case MO_TOKEN_INTEGER:      //  12345
        case MO_TOKEN_NAME:         //  max/min
            mo_push_unit(u->super.mo, protobuf_unit_reserved_id_new());
            u->state = 2;
            return MO_ACTION_RETRY;
        case ';'
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 2:
        if (',' == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }

        u->state = 1;
        return MO_ACTION_RETRY;
    break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "'syntax' 之后需要 '='"));
    return MO_ACTION_ERROR;
}


MO_EXTERN struct unit_t* protobuf_unit_reserved_new()
{
    struct protobuf_unit_reserved_t* unit = (struct protobuf_unit_reserved_t*)malloc(sizeof(struct protobuf_unit_reserved_t));
    unit->super.size    =   sizeof(struct protobuf_unit_reserved_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_reserved_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}



