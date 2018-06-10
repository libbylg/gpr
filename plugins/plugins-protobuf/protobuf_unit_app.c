
#include "protobuf.h"
#include "plugins/plugins-protobuf/protobuf_token.h"

#include <stdlib.h>

struct protobuf_unit_app_t
{
    struct unit_t   super;
    int             state;
};

static mo_action protobuf_unit_app_accept(struct unit_t*   n, struct token_t* t)
{
    struct protobuf_unit_app_t* u = (struct protobuf_unit_app_t*)n;

    if (0 == u->state)
    {
        switch (t->token)
        {
        case MO_TOKEN_syntax:
            mo_push_unit(u->super.mo, protobuf_unit_syntax_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_package:
            mo_push_unit(u->super.mo, protobuf_unit_package_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_import:
            mo_push_unit(u->super.mo, protobuf_unit_import_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_service:
            mo_push_unit(u->super.mo, protobuf_unit_service_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_message:
            mo_push_unit(u->super.mo, protobuf_unit_message_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_enum:
            mo_push_unit(u->super.mo, protobuf_unit_enum_new());
            u->state = 1;
            return MO_ACTION_RETRY;
        case MO_TOKEN_EOF:
            return MO_ACTION_COMPLETE;
        }
    }
    else
    if (1 == u->state)
    {
        if (';' == t->token)
        {
            u->state = 0;
            return MO_ACTION_NEEDMORE;
        }
    }

    mo_push_result(u->super.mo, mo_result_new("parser", 111, "unexpected token"));
    return MO_ACTION_ERROR;
}

MO_EXTERN struct unit_t* protobuf_unit_app_new()
{
    struct protobuf_unit_app_t* unit = (struct protobuf_unit_app_t*)malloc(sizeof(struct protobuf_unit_app_t));
    unit->super.size    =   sizeof(struct protobuf_unit_app_t);
    unit->super.prev    =   NULL;
    unit->super.mo      =   NULL;
    unit->super.del     =   free;
    unit->super.accept  =   protobuf_unit_app_accept;
    unit->state         =   0;
    return (struct unit_t*)(unit);
}



