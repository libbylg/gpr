#include "mo.h"

#include "plugins/plugins-protobuf/protobuf_token.h"

struct protobuf_unit_sevice_method_t
{
    struct unit_t   super;
    int             state;
};


static mo_action protobuf_unit_sevice_method_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_sevice_method_t* u = (struct protobuf_unit_sevice_method_t*)n;
    switch (u->state)
    {
    case 0: //  等 init / exit / rpc 可选
        switch (t->token)
        {
        case MO_TOKEN_init:
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        case MO_TOKEN_exit:
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        case MO_TOKEN_rpc:
            u->state = 1;
            return MO_ACTION_NEEDMORE;
        }

        u->state = 1;
        return MO_ACTION_RETRY;
    break;
    case 1: //  等 NAME
        if (MO_TOKEN_NAME == t->token)
        {
            mo_push_unit(u->super.mo, protobuf_unit_param_list_new());
            u->state = 2;
            return MO_ACTION_RETRY;
        }
    break;
    case 2:
        if (MO_TOKEN_returns == t->token)   //  returns 可选
        {
            mo_push_unit(u->super.mo, protobuf_unit_param_list_new());
            u->state = 3;
            return MO_ACTION_NEEDMORE;
        }

        u->state = 3;
        return MO_ACTION_RETRY;
    break;
    case 3:
        if (';' == t->token)    //  分号可选
        {
            mo_pop_unit(u->super.mo);
            return MO_ACTION_NEEDMORE;
        }

        mo_pop_unit(u->super.mo);
        return MO_ACTION_RETRY;
    break;
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "'syntax' 之后需要 '='"));
    return MO_ACTION_ERROR;
}


MO_EXTERN struct unit_t* protobuf_unit_sevice_method_new()
{
    struct protobuf_unit_sevice_method_t* unit = (struct protobuf_unit_sevice_method_t*)malloc(sizeof(struct protobuf_unit_sevice_method_t));
    unit->super.size    =   sizeof(struct protobuf_unit_sevice_method_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_sevice_method_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}






