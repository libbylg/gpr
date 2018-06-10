#include "mo.h"
#include <stdlib.h>
#include "protobuf_token.h"

struct protobuf_unit_option_t
{
    struct unit_t   super;
    int             state;
};


static mo_action protobuf_unit_option_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_option_t* u = (struct protobuf_unit_option_t*)n;

    switch (u->state)
    {
    case 0: //  option
        if (MO_TOKEN_option == t->token)
        {
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 1:
        if (MO_TOKEN_NAME == t->token)
        {
            u->state = 2;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 2: //  等 = 
        if ('=' == t->token)
        {
            u->state = 3;
            return MO_ACTION_NEEDMORE;
        }
    break;
    case 3:
        switch (t->token)
        {
        case MO_TOKEN_INTEGER:
        case MO_TOKEN_STRING:
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }
    break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "'syntax' 之后需要 '='"));
    return MO_ACTION_ERROR;
}


MO_EXTERN struct unit_t* protobuf_unit_option_new()
{
    struct protobuf_unit_option_t* unit = (struct protobuf_unit_option_t*)malloc(sizeof(struct protobuf_unit_option_t));
    unit->super.size    =   sizeof(struct protobuf_unit_option_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_option_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}



